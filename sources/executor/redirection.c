/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:15:40 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/07 14:05:04 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Sets up output redirection for a command.
 * @param param Pointer to the process command arguments structure.
 * @return 1 if STDOUT was redirected to a file by set_redirection_to_files.
 *         0 if STDOUT was not redirected to a file by set_redirection_to_files.
 *
	-1 on failure (as reported by set_redirection_to_files or basic validation).

	* @note This function prepares the flags for output file redirection (O_APPEND or
 *       O_TRUNC) and then calls set_redirection_to_files. The return value of
 *       set_redirection_to_files dictates the outcome, which is crucial for
 *       pipeline logic in setup_child_io.
 */
int	setup_output_redirection(t_process_command_args *param)
{
	int	flags;
	int	redirect_status;

	if (!param || !param->cmd)
		return (-1);
	flags = O_WRONLY | O_CREAT;
	if (param->cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	redirect_status = set_redirection_to_files(param, &flags);
	return (redirect_status);
}

/**
 * @brief Sets up input redirection for a command.
 *
 * This function handles input sources in the following order of priority:
 * 1. Heredoc: If `cmd->heredoc_fd` is valid (>= 0).
 * 2. Input file: If `cmd->input_file` is specified.
 * 3. Pre-existing input fd: If `cmd->fd_in` is already set to a valid fd
 *    (e.g., by a previous stage in a pipeline, though typically the shell's
 *    main loop handles stdin for pipes before calling process_command).
 * If none of these are specified, STDIN_FILENO remains unchanged.
 *
 * @param cmd The command structure, containing heredoc_fd, input_file,
	and fd_in.

	* @param env Array of environment variables (currently unused in this function).
 * @param last_exit Last exit status (currently unused in this function).
 * @return 0 on success, -1 on failure (e.g., file open error, dup2 error).
 */
int	setup_input_redirection(t_command *cmd, char **env, int last_exit)
{
	int	fd;

	(void)env;
	(void)last_exit;
	if (!cmd)
		return (-1);
	if (cmd->heredoc_fd >= 0)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) < 0)
		{
			perror("minishell: dup2 for heredoc STDIN failed");
			close(cmd->heredoc_fd);
			return (-1);
		}
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
		return (0);
	}
	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd < 0)
		{
			perror(cmd->input_file);
			return (-1);
		}
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			perror("minishell: dup2 for input file STDIN failed");
			close(fd);
			return (-1);
		}
		close(fd);
		return (0);
	}
	return (0);
}
