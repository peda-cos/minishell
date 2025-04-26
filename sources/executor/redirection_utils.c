/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 08:15:40 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/26 18:40:37 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	setup_file_input(char *input_file)
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
	if (cmd->heredoc_fd >= 0)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			close(cmd->heredoc_fd);
			return (-1);
		}
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
		return (0);
	}
	else if (cmd->heredoc_delim)
	{
		return (handle_heredoc(cmd->heredoc_delim, env, last_exit));
	}
	else if (cmd->input_file)
	{
		return (setup_file_input(cmd->input_file));
	}
	return (0);
}

t_token	*get_fd_redirect_token(char *target_file, t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == FILE_DESCRIPTOR
			&& ft_strcmp(tmp->next->next->content->value, target_file) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	dup_fd_std(int fd, char *filename, t_process_command_args *arg)
{
	t_token	*fd_redirect_token;

	fd_redirect_token = get_fd_redirect_token(filename, arg->tokens);
	if (fd_redirect_token
		&& ft_atoi(fd_redirect_token->content->value) == STDERR_FILENO)
		return (dup2(fd, STDERR_FILENO));
	return (dup2(fd, STDOUT_FILENO));
}

int	set_redirection_to_files(t_process_command_args *arg, int *flags)
{
	int		fd;
	int		index;
	char	**output_file_list;

	index = 0;
	output_file_list = arg->cmd->output_file_list;
	while (index < arg->cmd->amount_output_files)
	{
		fd = open(output_file_list[index], *flags, 0644);
		if (fd < 0)
		{
			perror(output_file_list[index]);
			return (-1);
		}
		if (dup_fd_std(fd, output_file_list[index], arg) < 0)
		{
			close(fd);
			return (-1);
		}
		close(fd);
		index++;
	}
	return (0);
}
