/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 11:59:20 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/08 23:29:49 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/**
 * @brief Prints a line of history with its line number
 * @param line_number The line number to print
 * @param line The line content to print
 * @note Prints the line number followed by two spaces and the line content
 */
void	print_history_line(int line_number, char *line)
{
	ft_putnbr_fd(line_number, STDOUT_FILENO);
	ft_putstr_fd("  ", STDOUT_FILENO);
	ft_putendl_fd(line, STDOUT_FILENO);
}

/**
 * @brief Displays the history entries from a file descriptor
 * @param fd The file descriptor to read from
 * @return int 0 on success, 1 on error
 * @note Reads lines from the FD and prints them with line numbers
 */
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

/**
 * @brief Displays the history entries from the history file
 * @param args The command-line arguments
 * @param last_exit The last exit status
 * @return int 0 on success, 1 on error
 * @note Opens the history file for reading and displays its contents
 */
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
	manager_file_descriptors(ADD, fd);
	display_history_entries(fd);
	close(fd);
	return (0);
}
