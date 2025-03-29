/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:19:21 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/29 15:15:49 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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

void	child_process(t_command *cmd, int pipefd[2], char **env, int *last_exit)
{
	if (!cmd || !env || !last_exit)
		exit(1);
	reset_signals();
	if (cmd->next && pipefd[1] > 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	if (setup_input_redirection(cmd) < 0 || setup_output_redirection(cmd) < 0)
		exit(1);
	if (cmd->argc <= 0)
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, &env, last_exit));
	exit(execute_external(cmd, env));
}

void	parent_process(t_command *cmd, int pipefd[2], pid_t pid, int *last_exit)
{
	int	status;

	if (!cmd || !last_exit)
		return ;
	if (cmd->next && pipefd[0] > 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}
	waitpid(pid, &status, 0);
	*last_exit = WEXITSTATUS(status);
}

int	process_command(t_command *cmd, char **env, int *last_exit)
{
	int		pipefd[2];
	pid_t	pid;

	if (!cmd || !env || !last_exit)
		return (-1);
	if (setup_pipe(cmd, pipefd) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	else if (pid == 0)
		child_process(cmd, pipefd, env, last_exit);
	else
		parent_process(cmd, pipefd, pid, last_exit);
	return (0);
}
