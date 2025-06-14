/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_wait.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:17:38 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/14 15:04:18 by jlacerda         ###   ########.fr       */
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
		ft_putchar_fd(BREAKLINE_CHR, STDOUT_FILENO);
	else if (WTERMSIG(status) == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
}

/**
 * @brief Checks if any child process was signaled and sets exit status
 * @param left_status Pointer to the left child's status
 * @param right_status Pointer to the right child's status
 * @param exit_status Pointer to store the exit status
 * @return 1 if a child was signaled, 0 otherwise
 * @note Updates exit status based on signal termination
 */
static int	get_status_signaled(int *left_status, int *right_status,
	int *exit_status)
{
	int	signaled;

	signaled = 0;
	if (WIFSIGNALED(*left_status))
	{
		signaled = 1;
		*exit_status = WTERMSIG(*left_status) + 128;
	}
	else if (WIFSIGNALED(*right_status))
	{
		signaled = 1;
		*exit_status = WTERMSIG(*right_status) + 128;
	}
	else
		*exit_status = WEXITSTATUS(*right_status);
	return (signaled);
}

/**
 * @brief Waits for child processes to finish and restores file descriptors
 * @param fildes Array of file descriptors for the pipe
 * @param forks Array of PIDs for the child processes
 * @param last_exit Pointer to store the last exit status
 * @param is_last_pipe Flag indicating if this is the last pipe
 * @return void
 * @note Closes pipe file descriptors and waits for both child processes
 */
void	wait_restore_fds_pipe(int *fildes, int *forks,
	int *last_exit, int is_last_pipe)
{
	int	left_status;
	int	right_status;
	int	exit_status;
	int	signaled;

	close(fildes[0]);
	close(fildes[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(forks[0], &left_status, 0);
	waitpid(forks[1], &right_status, 0);
	signal(SIGINT, SIG_DFL);
	signaled = get_status_signaled(&left_status, &right_status, &exit_status);
	if (signaled && is_last_pipe)
		handle_signal_termination(left_status, &exit_status);
	*last_exit = exit_status;
}
