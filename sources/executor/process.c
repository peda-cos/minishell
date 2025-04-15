/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:19:21 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/14 11:57:13 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	setup_child_io(t_command *cmd, int pipefd[2])
{
	if (cmd->next && pipefd[1] > 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	if (setup_input_redirection(cmd) < 0 || setup_output_redirection(cmd) < 0)
		return (-1);
	return (0);
}

static int	handle_empty_command(t_command *cmd)
{
	char	buffer[4096];
	ssize_t	bytes_read;

	if (cmd->argc <= 0)
	{
		if (cmd->output_file != NULL)
		{
			bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
			while (bytes_read > 0)
			{
				write(STDOUT_FILENO, buffer, bytes_read);
				bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
			}
		}
		return (1);
	}
	return (0);
}

void	child_process(t_process_command_args *param)
{
	if (!param->cmd || !param->env || !(param->last_exit))
		exit_free(1, param->env, param->cmd, param->tokens);
	reset_signals();
	if (setup_child_io(param->cmd, param->pipefd) < 0)
		exit_free(1, param->env, param->cmd, param->tokens);
	if (handle_empty_command(param->cmd))
		exit_free(0, param->env, param->cmd, param->tokens);
	if (is_builtin(param->cmd->args[0]))
		exit_free(execute_builtin(param->cmd, &param->env, param->last_exit),
			param->env, param->cmd, param->tokens);
	exit_free(execute_external(param->cmd, param->env), param->env, param->cmd,
		param->tokens);
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
	if (!param->cmd->next)
	{
		waitpid(param->pid, &status, 0);
		*(param->last_exit) = WEXITSTATUS(status);
	}
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
