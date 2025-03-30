/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:19:21 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/30 02:07:26 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void exit_child_process(int signal, t_process_command_args *param)
{
	free_env(param->env);
	free_commands(param->cmd);
	free_tokens(param->tokens);
	exit(signal);
}

int	setup_pipe(t_command *cmd, int pipefd[2])
{
	if (!cmd)
		return (-1);
	if (cmd->next)
	{
		if (pipe(pipefd) < 0)
		{
			perror("pipe");
			return (-1);
		}
	}
	else
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
	}
	return (0);
}

void	child_process(t_process_command_args *param) 
{
	if (!param->cmd || !param->env || !(param->last_exit))
	exit_child_process(1, param);
	reset_signals();
	if (param->cmd->next && param->pipefd[1] > 0)
	{
		dup2(param->pipefd[1], STDOUT_FILENO);
		close(param->pipefd[0]);
		close(param->pipefd[1]);
	}
	if (setup_input_redirection(param->cmd) < 0 
		|| setup_output_redirection(param->cmd) < 0)
	exit_child_process(1, param);
	if (param->cmd->argc <= 0)
	exit_child_process(0, param);
	if (is_builtin(param->cmd->args[0]))
		exit_child_process(execute_builtin(
			param->cmd, &param->env, (param->last_exit)), param);
	exit_child_process(execute_external(
		param->cmd, param->env), param);
}

void	parent_process(t_process_command_args *param)
{
	int	status;

	if (!param->cmd || !(param->last_exit))
	return ;
	if (param->cmd->next && param->pipefd[0] > 0)
	{
		close(param->pipefd[1]);
		dup2(param->pipefd[0], STDIN_FILENO);
		close(param->pipefd[0]);
	}
	waitpid(param->pid, &status, 0);
	*(param->last_exit) = WEXITSTATUS(status);
}

int	process_command(t_command *cmd,
	char **env, int *last_exit, t_token	*tokens)
{
	int		pipefd[2];
	pid_t	pid;
	t_process_command_args	args;

	if (!cmd || !env || !last_exit)
		return (-1);
	if (setup_pipe(cmd, pipefd) < 0)
		return (-1);
	pid = fork();
	args.cmd	= cmd;
	args.env	= env;
	args.pid	= pid;
	args.tokens	= tokens;
	args.last_exit	= last_exit;
	args.pipefd[0]	= pipefd[0];
	args.pipefd[1]	= pipefd[1];
	if (pid < 0)
		return (perror("fork"), -1);
	else if (pid == 0)
		child_process(&args);
	else
		parent_process(&args);
	return (0);
}
