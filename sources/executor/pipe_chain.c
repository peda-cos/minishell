/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_chain.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:17:46 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/14 14:51:20 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Sets up signal handlers for child process
 * @return void
 * @note Resets signal handlers to default for child process
 */
static void	setup_chain_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * @brief Handles command execution in pipe chain
 * @param cmd_start Starting command in the chain
 * @param args Process command arguments
 * @return void
 * @note Executes command with proper redirection setup
 */
static void	execute_chain_command(t_command *cmd_start,
		t_process_command_args *args)
{
	args->cmd = cmd_start;
	if (setup_output_redirection(args) < 0)
		exit_free(1, args->env, args->head, args->tokens);
	if (setup_input_redirection(args->cmd, *args->env, *args->last_exit) < 0)
		exit_free(1, args->env, args->head, args->tokens);
	if (args->cmd->argc <= 0)
		exit_free(0, args->env, args->head, args->tokens);
	if (is_builtin(args->cmd->args[0]))
		exit_free(execute_builtin(args), args->env, args->head, args->tokens);
	else
		exit_free(execute_external(args->cmd, *args->env,
				has_redirect_out(args->tokens)), args->env, args->head,
			args->tokens);
}

/**
 * @brief Handles fork process execution for pipe chains
 * @param fildes Array of pipe file descriptors from previous pipe
 * @param cmd_start Starting command in the chain
 * @param args Process command arguments
 * @return void
 * @note Redirects stdin from previous pipe and handles remaining chain
 */
void	fork_process_pipe_chain(int *fildes, t_command *cmd_start,
		t_process_command_args *args)
{
	int	is_last_pipe;

	setup_chain_signals();
	dup2(fildes[0], STDIN_FILENO);
	close(fildes[0]);
	close(fildes[1]);
	if (cmd_start->next)
	{
		args->cmd = cmd_start;
		is_last_pipe = !cmd_start->next->next;
		handle_pipe(cmd_start, cmd_start->next, args, is_last_pipe);
	}
	else
		execute_chain_command(cmd_start, args);
	exit_free(0, args->env, args->head, args->tokens);
}
