/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_wait.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:17:38 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/05 16:22:13 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Handles signal termination for child processes
 * @param status The exit status from waitpid
 * @param exit_status Pointer to store the calculated exit status
 * @return void
 * @note Processes signal termination and prints appropriate messages
 */
static void	handle_signal_termination(int status, int *exit_status)
{
	*exit_status = WTERMSIG(status) + 128;
	if (WTERMSIG(status) == SIGINT)
		ft_putchar_fd('\n', STDOUT_FILENO);
	else if (WTERMSIG(status) == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
}

/**
 * @brief Waits for child processes and restores file descriptors
 * @param fildes Array of pipe file descriptors
 * @param forks Array of fork PIDs
 * @param last_exit Pointer to store exit status
 * @return void
 * @note Closes pipe file descriptors and waits for both children
 */
void	wait_restore_fds_pipe(int *fildes, int *forks, int *last_exit)
{
	int	status;
	int	exit_status;

	close(fildes[0]);
	close(fildes[1]);
	waitpid(forks[0], &status, 0);
	waitpid(forks[1], &status, 0);
	if (WIFSIGNALED(status))
		handle_signal_termination(status, &exit_status);
	else
		exit_status = WEXITSTATUS(status);
	*last_exit = exit_status;
}
