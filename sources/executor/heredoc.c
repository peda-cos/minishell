/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:12:39 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/24 22:59:37 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	*append_to_buffer(char *buffer, char *line)
{
	char	*temp;
	char	*result;

	temp = buffer;
	buffer = ft_strjoin(buffer, line);
	free(temp);
	if (!buffer)
		return (NULL);
	temp = buffer;
	result = ft_strjoin(buffer, "\n");
	free(temp);
	return (result);
}

static int	is_quoted_delimiter(char *delim)
{
	int	len;

	len = ft_strlen(delim);
	return (len >= 2 && delim[0] == '\'' && delim[len - 1] == '\'');
}

static char	*process_heredoc_line(char *line, char **env, int last_exit,
		int expand_vars)
{
	char	*expanded;
	int		has_error;

	has_error = FALSE;
	if (!expand_vars)
		return (ft_strdup(line));
	expanded = expand_variable(line, env, last_exit, &has_error);
	return (expanded);
}

static char	*read_heredoc_content(char *delim, char **env, int last_exit)
{
	char	*line;
	char	*buffer;
	char	*processed_line;
	int		expand_vars;
	char	*stripped_delim;

	buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	expand_vars = !is_quoted_delimiter(delim);
	if (!expand_vars && delim[0] == '\'')
	{
		stripped_delim = ft_substr(delim, 1, ft_strlen(delim) - 2);
		if (!stripped_delim)
		{
			free(buffer);
			return (NULL);
		}
	}
	else
		stripped_delim = ft_strdup(delim);
	if (!stripped_delim)
	{
		free(buffer);
		return (NULL);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, stripped_delim))
		{
			free(line);
			free(stripped_delim);
			break ;
		}
		processed_line = process_heredoc_line(line, env, last_exit,
				expand_vars);
		free(line);
		if (!processed_line)
		{
			free(buffer);
			free(stripped_delim);
			return (NULL);
		}
		buffer = append_to_buffer(buffer, processed_line);
		free(processed_line);
		if (!buffer)
		{
			free(stripped_delim);
			return (NULL);
		}
	}
	return (buffer);
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

char	*read_heredoc_content_to_buffer(char *delim)
{
	char	*line;
	char	*buffer;
	char	*tmp;

	buffer = ft_strdup("");
	while (1)
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

void	preprocess_heredocs(t_command *cmd_list)
{
	t_command	*cmd;
	int			pipefd[2];
	char		*buffer;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->heredoc_delim)
		{
			if (pipe(pipefd) < 0)
			{
				perror("pipe");
				return ;
			}
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
