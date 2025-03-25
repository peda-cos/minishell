/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:12:39 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/25 08:16:51 by peda-cos         ###   ########.fr       */
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

static char	*read_heredoc_content(char *delim)
{
	char	*line;
	char	*buffer;

	buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, delim))
		{
			free(line);
			break ;
		}
		buffer = append_to_buffer(buffer, line);
		if (!buffer)
			return (NULL);
		free(line);
	}
	return (buffer);
}

int	handle_heredoc(char *delim)
{
	int		pipefd[2];
	char	*buffer;

	if (!delim || pipe(pipefd) < 0)
		return (-1);
	buffer = read_heredoc_content(delim);
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
