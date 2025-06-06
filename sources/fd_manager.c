/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_manager.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 00:37:04 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/20 20:57:08 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Restores standard file descriptors to their original state
 * @return void
 */
void	restore_std_file_descriptors(void)
{
	int	std_fds[3];
	int	i;
	int	test_status;

	std_fds[0] = STDIN_FILENO;
	std_fds[1] = STDOUT_FILENO;
	std_fds[2] = STDERR_FILENO;
	i = 0;
	while (i < 3)
	{
		test_status = write(std_fds[i], NULL, 0);
		if (test_status != -1)
			dup2(std_fds[i], std_fds[i]);
		i++;
	}
}

/**
 * @brief Initializes the file descriptor list
 * @param fd_list Pointer to the file descriptor list
 * @param fd_index Pointer to the file descriptor index
 * @return void
 */
static void	init_fd_list(int **fd_list, int *fd_index)
{
	*fd_list = malloc(sizeof(int) * FD_LIST_SIZE);
	*fd_index = 0;
	while (*fd_index < FD_LIST_SIZE)
		(*fd_list)[(*fd_index)++] = -1;
	*fd_index = 0;
}

/**
 * @brief Closes and cleans up all file descriptors
 * @param fd_list Pointer to the file descriptor list
 * @param fd_index Pointer to the file descriptor index
 * @return void
 */
static void	cleanup_fd_list(int **fd_list, int *fd_index)
{
	*fd_index = 0;
	while (*fd_index < FD_LIST_SIZE)
	{
		if ((*fd_list)[*fd_index] > STDERR_FILENO)
		{
			close((*fd_list)[*fd_index]);
			(*fd_list)[*fd_index] = -1;
		}
		(*fd_index)++;
	}
	*fd_index = 0;
	free(*fd_list);
	*fd_list = NULL;
}

/**
 * @brief Manages file descriptors for the shell
 * @param fd_type Type of operation (ADD or FREE)
 * @param fd File descriptor to manage
 * @return void
 * @note Allocates memory for file descriptors and manages their lifecycle
 */
void	manager_file_descriptors(t_manager_fd fd_type, int fd)
{
	static int	fd_index = 0;
	static int	*fd_list = NULL;

	if (fd_list == NULL)
		init_fd_list(&fd_list, &fd_index);
	if (fd_type == ADD)
		fd_list[fd_index++] = fd;
	else if (fd_type == FREE)
		cleanup_fd_list(&fd_list, &fd_index);
}
