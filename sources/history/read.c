/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:19:54 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 19:42:00 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/**
 * @brief Validates the line read from the file descriptor
 * @param line The line to validate
 * @param bytes_read The number of bytes read
 * @return char* The validated line or NULL if empty
 * @note Frees the line if it is empty and
	* bytes_read is less than or equal to 0
 */
static char	*validate_line(char *line, ssize_t bytes_read)
{
	if (bytes_read <= 0 && ft_strlen(line) == 0)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

/**
 * @brief Reads a line from a file descriptor
 * @param fd The file descriptor to read from
 * @return char* The line read or NULL on error/EOF
 * @note Allocates memory for the line and returns a pointer to it
 *       Caller is responsible for freeing the returned string
 */
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

/**
 * @brief Loads lines from a file descriptor into history
 * @param fd The file descriptor to read from
 * @return int The number of lines loaded or -1 on error
 * @note Reads lines from the file descriptor and adds them to history
 */
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

/**
 * @brief Loads history from file into readline
 * @return int 0 on success, 1 on error
 * @note Opens the history file for
	* reading and loads its contents into history
 */
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
