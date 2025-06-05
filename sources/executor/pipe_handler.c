/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:05:09 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/05 16:05:14 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Handles fork process execution for pipe
 * @param fildes Array of pipe file descriptors
 * @param cmd_left Left command in the pipe
 * @param cmd_right Right command in the pipe
 * @param left_right Flag to indicate which side (0=left, 1=right)
 * @param args Process command arguments
 * @return void
 * @note Sets up proper file descriptor redirection and executes command
 */
static void	fork_process_pipe(int *fildes, t_command *cmd_left,
		t_command *cmd_right, int left_right, t_process_command_args *args)
{
	struct sigaction	sa;
	int					exec_status_signal;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	if (!left_right)
	{
		dup2(fildes[1], STDOUT_FILENO);
		args->cmd = cmd_left;
	}
	else if (left_right == 1)
	{
		dup2(fildes[0], STDIN_FILENO);
		args->cmd = cmd_right;
	}
	close(fildes[0]);
	close(fildes[1]);
	if (setup_output_redirection(args) < 0)
		exit_free(1, args->env, args->head, args->tokens);
	if (setup_input_redirection(args->cmd, *args->env, *args->last_exit) < 0)
		exit_free(1, args->env, args->head, args->tokens);
	if (args->cmd->argc <= 0)
		exit_free(0, args->env, args->head, args->tokens);
	if (is_builtin(args->cmd->args[0]))
		exit_free(execute_builtin(args), args->env, args->head, args->tokens);
	else
	{
		exec_status_signal = execute_external(args->cmd, *args->env,
				has_redirect_out(args->tokens));
		exit_free(exec_status_signal, args->env, args->head, args->tokens);
	}
}

/**
 * @brief Handles fork process execution for pipe chains
 * @param fildes Array of pipe file descriptors from previous pipe
 * @param cmd_start Starting command in the chain
 * @param args Process command arguments
 * @return void
 * @note Redirects stdin from previous pipe and handles remaining chain
 */
static void	fork_process_pipe_chain(int *fildes, t_command *cmd_start,
		t_process_command_args *args)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	dup2(fildes[0], STDIN_FILENO);
	close(fildes[0]);
	close(fildes[1]);
	if (cmd_start->next)
	{
		args->cmd = cmd_start;
		handle_pipe(cmd_start, cmd_start->next, args);
	}
	else
	{
		args->cmd = cmd_start;
		if (setup_output_redirection(args) < 0)
			exit_free(1, args->env, args->head, args->tokens);
		if (setup_input_redirection(args->cmd, *args->env,
				*args->last_exit) < 0)
			exit_free(1, args->env, args->head, args->tokens);
		if (args->cmd->argc <= 0)
			exit_free(0, args->env, args->head, args->tokens);
		if (is_builtin(args->cmd->args[0]))
			exit_free(execute_builtin(args), args->env, args->head,
				args->tokens);
		else
			exit_free(execute_external(args->cmd, *args->env,
					has_redirect_out(args->tokens)), args->env, args->head,
				args->tokens);
	}
	exit_free(0, args->env, args->head, args->tokens);
}

/**
 * @brief Waits for child processes and restores file descriptors
 * @param fildes Array of pipe file descriptors
 * @param forks Array of fork PIDs
 * @param last_exit Pointer to store exit status
 * @return void
 * @note Closes pipe file descriptors and waits for both children
 */
static void	wait_restore_fds_pipe(int *fildes, int *forks, int *last_exit)
{
	int	status;
	int	exit_status;

	close(fildes[0]);
	close(fildes[1]);
	waitpid(forks[0], &status, 0);
	waitpid(forks[1], &status, 0);
	if (WIFSIGNALED(status))
	{
		exit_status = WTERMSIG(status) + 128;
		if (WTERMSIG(status) == SIGINT)
			ft_putchar_fd('\n', STDOUT_FILENO);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
	}
	else
		exit_status = WEXITSTATUS(status);
	*last_exit = exit_status;
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
	int		fildes[2];
	pid_t	forks[2];

	if (!pipe(fildes))
	{
		forks[0] = fork();
		if (forks[0] < 0)
		{
			perror("fork");
			return (1);
		}
		if (forks[0] == 0)
			fork_process_pipe(fildes, cmd_left, cmd_right, 0, args);
		forks[1] = fork();
		if (forks[1] < 0)
		{
			perror("fork");
			return (1);
		}
		if (forks[1] == 0)
		{
			if (cmd_right->next)
				fork_process_pipe_chain(fildes, cmd_right, args);
			else
				fork_process_pipe(fildes, cmd_left, cmd_right, 1, args);
		}
		wait_restore_fds_pipe(fildes, forks, args->last_exit);
	}
	else
	{
		perror("pipe");
		*args->last_exit = 1;
		return (1);
	}
	return (0);
}
