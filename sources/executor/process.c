/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:19:21 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/07 22:12:02 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	child_process(t_process_command_args *param)
{
	if (!param->cmd || !param->env || !(param->last_exit))
		exit_free(1, param->env, param->cmd, param->tokens);
	reset_signals();
	if (param->cmd->next && param->pipefd[1] > 0)
	{
		dup2(param->pipefd[1], STDOUT_FILENO);
		close(param->pipefd[0]);
		close(param->pipefd[1]);
	}
	if (setup_input_redirection(param->cmd) < 0
		|| setup_output_redirection(param->cmd) < 0)
		exit_free(1, param->env, param->cmd, param->tokens);
	if (param->cmd->argc <= 0)
		exit_free(0, param->env, param->cmd, param->tokens);
	if (is_builtin(param->cmd->args[0]))
		exit_free(execute_builtin(param->cmd, &param->env, param->last_exit),
			param->env, param->cmd, param->tokens);
	exit_free(execute_external(param->cmd, param->env),
		param->env, param->cmd, param->tokens);
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

int	process_command(t_command *cmd, char **env, int *last_exit, t_token *tokens)
{
	int						pipefd[2];
	t_process_command_args	args;

	if (!cmd || !env || !last_exit)
		return (-1);
	if (setup_pipe(cmd, pipefd) < 0)
		return (-1);
	args.cmd = cmd;
	args.env = env;
	args.tokens = tokens;
	args.last_exit = last_exit;
	args.pid = fork();
	args.pipefd[0] = pipefd[0];
	args.pipefd[1] = pipefd[1];
	if (args.pid < 0)
	{
		perror("fork");
		return (-1);
	}
	else if (args.pid == 0)
		child_process(&args);
	else
		parent_process(&args);
	return (0);
}
