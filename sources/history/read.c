/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:19:54 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/13 15:36:06 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

static char	*validate_line(char *line, ssize_t bytes_read)
{
	if (bytes_read <= 0 && ft_strlen(line) == 0)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

char	*read_line_from_fd(int fd)
{
	char	buffer[2];
	char	*line;
	char	*temp;
	ssize_t	bytes_read;

	line = ft_strdup("");
	if (!line)
		return (NULL);
	while (1)
	{
		bytes_read = read(fd, buffer, 1);
		if (bytes_read <= 0 || buffer[0] == '\n')
			break ;
		buffer[1] = '\0';
		temp = line;
		line = ft_strjoin(line, buffer);
		free(temp);
		if (!line)
			return (NULL);
	}
	return (validate_line(line, bytes_read));
}

int	load_history_from_file(int fd)
{
	char	*line;
	int		count;

	count = 0;
	if (fd < 0)
		return (-1);
	while (count < HISTORY_MAX_ENTRIES)
	{
		line = read_line_from_fd(fd);
		if (!line)
			break ;
		add_history(line);
		free(line);
		count++;
	}
	return (count);
}

int	load_history(void)
{
	int	fd;
	int	result;

	fd = open_history_file_read();
	if (fd < 0)
		return (1);
	result = load_history_from_file(fd);
	close(fd);
	return (result < 0);
}
