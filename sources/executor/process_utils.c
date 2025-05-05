/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:56:05 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 18:54:23 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Sets up a pipe for the command execution
 * @param cmd The command structure to be executed
 * @param pipefd The file descriptors for the pipe
 * @return 0 on success, -1 on error
 * @note Creates a pipe if the command has a next command in the chain
 */
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

/**
 * @brief Sets up the input and output redirection for the command
 * @param arg The command arguments and environment variables
 * @return 0 on success, -1 on error
 * @note Redirects the standard input and output file descriptors as needed
 */
int	setup_child_io(t_process_command_args *arg)
{
	if (arg->cmd->next && arg->pipefd[1] > 0)
	{
		dup2(arg->pipefd[1], STDOUT_FILENO);
		close(arg->pipefd[0]);
		close(arg->pipefd[1]);
	}
	if (setup_output_redirection(arg) < 0)
		return (-1);
	if (setup_input_redirection(arg->cmd, *arg->env, *arg->last_exit) < 0)
		return (-1);
	return (0);
}
