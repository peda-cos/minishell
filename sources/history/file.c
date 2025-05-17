/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:22:25 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/17 18:04:28 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/**
 * @brief Gets the full path to the history file
 * @return Dynamically allocated string containing
	* the path to the history file, or NULL on failure
 * @note Allocates memory for the path string
 */
char	*get_history_file_path(void)
{
	char	*home;
	char	*path;
	char	*temp;

	home = getenv("HOME");
	if (!home)
		return (NULL);
	path = ft_strjoin(home, "/");
	if (!path)
		return (NULL);
	temp = path;
	path = ft_strjoin(path, HISTORY_FILE);
	free(temp);
	return (path);
}

/**
 * @brief Opens history file for reading
 * @return File descriptor for the opened file, or -1 on error
 * @note Opens the file in read-only mode
 *       Returns -1 if path cannot be determined or file cannot be opened
 */
int	open_history_file_read(void)
{
	char	*path;
	int		fd;

	path = get_history_file_path();
	if (!path)
		return (-1);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		perror(path);
		free(path);
		return (-1);
	}
	close(fd);
	free(path);
	return (fd);
}

/**
 * @brief Opens history file for writing
 * @return File descriptor for the opened file, or -1 on error
 * @note Opens the file in write-only mode,
	* creating it if it doesn't exist and appending to it if it does
 */
int	open_history_file_write(void)
{
	char	*path;
	int		fd;

	path = get_history_file_path();
	if (!path)
		return (-1);
	fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(path);
		free(path);
		return (-1);
	}
	close(fd);
	free(path);
	return (fd);
}
