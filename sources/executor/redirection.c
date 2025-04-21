/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:15:40 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/21 14:32:20 by jlacerda         ###   ########.fr       */
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

static int	has_fd_redirect_to_stderr(t_process_command_args *arg)
{
	t_token	*tmp;
	int		redirect_fd;

	tmp = arg->tokens;
	redirect_fd = FALSE;
	while (tmp && ft_strcmp(tmp->content->value, arg->cmd->output_file) != 0)
	{
		if (tmp->type == FILE_DESCRIPTOR
			&& ft_atoi(tmp->content->value) == STDERR_FILENO)
		{
			arg->has_fd_redirect_to_stderr = TRUE;
			redirect_fd = TRUE;
			break ;
		}
		tmp = tmp->next;
	}
	return (redirect_fd);
}

int	setup_output_redirection(t_process_command_args *param)
{
	int	fd;
	int	flags;

	if (!param->cmd->output_file)
		return (0);
	flags = O_WRONLY | O_CREAT;
	if (param->cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(param->cmd->output_file, flags, 0644);
	if (fd < 0)
	{
		perror(param->cmd->output_file);
		return (-1);
	}
	if (has_fd_redirect_to_stderr(param))
		dup2(fd, STDERR_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}
