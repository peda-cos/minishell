/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 11:59:37 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/13 15:21:28 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

# include "minishell.h"

# define HISTORY_FILE ".minishell_history"
# define HISTORY_MAX_ENTRIES 1000

/**
 * @brief Gets the full path to the history file
 *
 * @return char* Path or NULL on error
 */
char	*get_history_file_path(void);

/**
 * @brief Reads a line from a file descriptor
 *
 * @param fd File descriptor to read from
 * @return char* Line read or NULL on error/EOF
 */
char	*read_line_from_fd(int fd);

/**
 * @brief Opens history file for reading
 *
 * @return int File descriptor or -1 on error
 */
int		open_history_file_read(void);

/**
 * @brief Opens history file for writing
 *
 * @return int File descriptor or -1 on error
 */
int		open_history_file_write(void);

/**
 * @brief Loads lines from file into history
 *
 * @param fd File descriptor
 * @return int Number of lines loaded or -1 on error
 */
int		load_history_from_file(int fd);

/**
 * @brief Loads history from file into readline
 *
 * @return int 0 on success, 1 on error
 */
int		load_history(void);

/**
 * @brief Saves a command to the history file
 *
 * @param cmd The command to save
 * @return int 0 on success, 1 on error
 */
int		save_command_to_history(char *cmd);

/**
 * @brief Prints a history line with number
 *
 * @param line_number The line number to display
 * @param line The command line to display
 */
void	print_history_line(int line_number, char *line);

/**
 * @brief Builtin command to display command history
 *
 * @param args Command arguments
 * @param last_exit Exit status of the last command
 * @return int 0 on success, 1 on error
 */
int		builtin_history(char **args, int *last_exit);

#endif
