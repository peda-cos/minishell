/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:05:09 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/05 16:22:15 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Creates pipe and handles first fork for left command
 * @param fildes Array to store pipe file descriptors
 * @param pipe_info Structure containing pipe and command information
 * @param args Process command arguments
 * @return Fork PID for left process, or -1 on error
 * @note Creates pipe and forks first child process
 */
static pid_t	create_left_fork(int *fildes, t_pipe_info *pipe_info,
		t_process_command_args *args)
{
	pid_t	fork_pid;

	if (pipe(fildes) < 0)
	{
		perror("pipe");
		return (-1);
	}
	fork_pid = fork();
	if (fork_pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (fork_pid == 0)
	{
		pipe_info->fildes = fildes;
		pipe_info->left_right = 0;
		fork_process_pipe(pipe_info, args);
	}
	return (fork_pid);
}

/**
 * @brief Creates second fork for right command
 * @param fildes Array of pipe file descriptors
 * @param pipe_info Structure containing pipe and command information
 * @param args Process command arguments
 * @return Fork PID for right process, or -1 on error
 * @note Forks second child process for right side of pipe
 */
static pid_t	create_right_fork(int *fildes, t_pipe_info *pipe_info,
		t_process_command_args *args)
{
	pid_t	fork_pid;

	fork_pid = fork();
	if (fork_pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (fork_pid == 0)
	{
		if (pipe_info->cmd_right->next)
			fork_process_pipe_chain(fildes, pipe_info->cmd_right, args);
		else
		{
			pipe_info->fildes = fildes;
			pipe_info->left_right = 1;
			fork_process_pipe(pipe_info, args);
		}
	}
	return (fork_pid);
}

/**
 * @brief Handles pipe execution between two commands
 * @param cmd_left Left command in the pipe
 * @param cmd_right Right command in the pipe
 * @param args Process command arguments
 * @return 0 on success, 1 on failure
 * @note Creates pipe, forks two processes, and handles execution
 */
int	handle_pipe(t_command *cmd_left, t_command *cmd_right,
		t_process_command_args *args)
{
	int			fildes[2];
	pid_t		forks[2];
	t_pipe_info	pipe_info;

	pipe_info.cmd_left = cmd_left;
	pipe_info.cmd_right = cmd_right;
	forks[0] = create_left_fork(fildes, &pipe_info, args);
	if (forks[0] < 0)
	{
		*args->last_exit = 1;
		return (1);
	}
	forks[1] = create_right_fork(fildes, &pipe_info, args);
	if (forks[1] < 0)
	{
		*args->last_exit = 1;
		return (1);
	}
	wait_restore_fds_pipe(fildes, forks, args->last_exit);
	return (0);
}
