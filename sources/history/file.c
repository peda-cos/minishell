/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:22:25 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/13 15:40:18 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

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

int	open_history_file_read(void)
{
	char	*path;
	int		fd;

	path = get_history_file_path();
	if (!path)
		return (-1);
	fd = open(path, O_RDONLY);
	free(path);
	return (fd);
}

int	open_history_file_write(void)
{
	char	*path;
	int		fd;

	path = get_history_file_path();
	if (!path)
		return (-1);
	fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	free(path);
	return (fd);
}
