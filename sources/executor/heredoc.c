/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:12:39 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/02 20:02:58 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	*process_heredoc_content(char *stripped_delim,
	int expand_vars, char **envs, int last_exit)
{
	char	*line;
	char	*buffer;
	char	*processed_line;

	buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	while (TRUE)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, stripped_delim))
			return (free(line), buffer);
		processed_line = process_heredoc_line(line,
				envs, last_exit, expand_vars);
		free(line);
		if (!processed_line)
			return (free(buffer), NULL);
		buffer = append_to_buffer(buffer, processed_line);
		free(processed_line);
		if (!buffer)
			return (NULL);
	}
}

static char	*read_heredoc_content_to_buffer(char *delim)
{
	char	*tmp;
	char	*line;
	char	*buffer;

	buffer = ft_strdup("");
	while (TRUE)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, delim))
		{
			free(line);
			break ;
		}
		tmp = buffer;
		buffer = ft_strjoin(buffer, line);
		free(tmp);
		tmp = buffer;
		buffer = ft_strjoin(buffer, "\n");
		free(tmp);
		free(line);
	}
	return (buffer);
}

static char	*read_heredoc_content(char *delim, char **env, int last_exit)
{
	char	*content;
	int		expand_vars;
	char	*stripped_delim;

	expand_vars = !is_quoted_delimiter(delim);
	stripped_delim = get_stripped_delim(expand_vars, delim);
	if (!stripped_delim)
		return (NULL);
	content = process_heredoc_content(stripped_delim,
			expand_vars, env, last_exit);
	free(stripped_delim);
	if (!content)
		return (NULL);
	return (content);
}

int	handle_heredoc(char *delim, char **env, int last_exit)
{
	int		pipefd[2];
	char	*buffer;

	if (!delim || pipe(pipefd) < 0)
		return (-1);
	signal(SIGINT, SIG_DFL);
	buffer = read_heredoc_content(delim, env, last_exit);
	if (!buffer)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	write(pipefd[1], buffer, ft_strlen(buffer));
	close(pipefd[1]);
	free(buffer);
	return (pipefd[0]);
}

void	preprocess_heredocs(t_command *cmd_list)
{
	t_command	*cmd;
	char		*buffer;
	int			pipefd[2];

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->heredoc_delim)
		{
			if (pipe(pipefd) < 0)
				return (perror("pipe"));
			buffer = read_heredoc_content_to_buffer(cmd->heredoc_delim);
			if (buffer)
			{
				write(pipefd[1], buffer, ft_strlen(buffer));
				free(buffer);
			}
			close(pipefd[1]);
			cmd->heredoc_fd = pipefd[0];
		}
		else
			cmd->heredoc_fd = -1;
		cmd = cmd->next;
	}
}
