/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:15:40 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/20 00:55:08 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	setup_heredoc_input(char *delim, char **env, int last_exit)
{
	int	hd_fd;

	hd_fd = handle_heredoc(delim, env, last_exit);
	if (hd_fd < 0)
		return (-1);
	if (dup2(hd_fd, STDIN_FILENO) < 0)
	{
		close(hd_fd);
		return (-1);
	}
	close(hd_fd);
	return (0);
}

static int	setup_file_input(char *input_file)
{
	int	fd;

	fd = open(input_file, O_RDONLY);
	if (fd < 0)
	{
		perror(input_file);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	setup_input_redirection(t_command *cmd, char **env, int last_exit)
{
	if (!cmd)
		return (-1);
	if (cmd->heredoc_delim)
		return (setup_heredoc_input(cmd->heredoc_delim, env, last_exit));
	else if (cmd->input_file)
		return (setup_file_input(cmd->input_file));
	return (0);
}

int	setup_output_redirection(t_command *cmd)
{
	int	fd;
	int	flags;

	if (!cmd->output_file)
		return (0);
	flags = O_WRONLY | O_CREAT;
	if (cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(cmd->output_file, flags, 0644);
	if (fd < 0)
	{
		perror(cmd->output_file);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}
