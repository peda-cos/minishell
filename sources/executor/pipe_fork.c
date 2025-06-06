/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:18:00 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/05 16:22:14 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Sets up signal handlers for child process
 * @return void
 * @note Resets signal handlers to default for child process
 */
static void	setup_child_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * @brief Sets up file descriptors for pipe process
 * @param fildes Array of pipe file descriptors
 * @param left_right Flag to indicate which side (0=left, 1=right)
 * @return void
 * @note Redirects stdin/stdout and closes pipe file descriptors
 */
static void	setup_pipe_fds(int *fildes, int left_right)
{
	if (!left_right)
		dup2(fildes[1], STDOUT_FILENO);
	else if (left_right == 1)
		dup2(fildes[0], STDIN_FILENO);
	close(fildes[0]);
	close(fildes[1]);
}

/**
 * @brief Executes command in pipe child process
 * @param args Process command arguments
 * @return void
 * @note Handles builtin and external command execution with proper exit
 */
static void	execute_pipe_command(t_process_command_args *args)
{
	int	exec_status_signal;

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
 * @brief Handles fork process execution for pipe
 * @param pipe_info Structure containing pipe info and commands
 * @param args Process command arguments
 * @return void
 * @note Sets up proper file descriptor redirection and executes command
 */
void	fork_process_pipe(t_pipe_info *pipe_info, t_process_command_args *args)
{
	setup_child_signals();
	setup_pipe_fds(pipe_info->fildes, pipe_info->left_right);
	if (!pipe_info->left_right)
		args->cmd = pipe_info->cmd_left;
	else if (pipe_info->left_right == 1)
		args->cmd = pipe_info->cmd_right;
	if (setup_output_redirection(args) < 0)
		exit_free(1, args->env, args->head, args->tokens);
	if (setup_input_redirection(args->cmd, *args->env, *args->last_exit) < 0)
		exit_free(1, args->env, args->head, args->tokens);
	execute_pipe_command(args);
}
