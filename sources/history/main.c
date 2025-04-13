/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 11:59:20 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/13 15:35:39 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

void	print_history_line(int line_number, char *line)
{
	ft_putnbr_fd(line_number, STDOUT_FILENO);
	ft_putstr_fd("  ", STDOUT_FILENO);
	ft_putendl_fd(line, STDOUT_FILENO);
}

static int	display_history_entries(int fd)
{
	char	*line;
	int		line_number;

	line_number = 1;
	while (1)
	{
		line = read_line_from_fd(fd);
		if (!line)
			break ;
		print_history_line(line_number, line);
		free(line);
		line_number++;
	}
	return (0);
}

int	builtin_history(char **args, int *last_exit)
{
	char	*path;
	int		fd;

	(void)args;
	(void)last_exit;
	path = get_history_file_path();
	if (!path)
		return (1);
	fd = open(path, O_RDONLY);
	free(path);
	if (fd < 0)
		return (1);
	display_history_entries(fd);
	close(fd);
	return (0);
}
