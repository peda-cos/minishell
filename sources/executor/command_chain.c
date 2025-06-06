/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_chain.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:18:06 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/05 16:28:48 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Initializes command arguments structure
 * @param args Pointer to the arguments structure
 * @param cmd_head The head of the command list
 * @param env Array of environment variables
 * @param exec_context Structure containing last_exit and tokens
 * @return void
 * @note Sets up the arguments structure with provided parameters
 */
static void	init_command_args(t_process_command_args *args, t_command *cmd_head,
		char ***env, t_execution_context *exec_context)
{
	args->env = env;
	args->head = cmd_head;
	args->tokens = exec_context->tokens;
	args->last_exit = exec_context->last_exit;
}

/**
 * @brief Processes a single command in the chain
 * @param cmd The command to process
 * @param args The command arguments structure
 * @return 0 on success, -1 on error, 1 to break the loop
 * @note Handles both pipe and single command processing
 */
static int	process_single_command(t_command *cmd, t_process_command_args *args)
{
	int	result;

	if (cmd->next)
	{
		result = handle_pipe(cmd, cmd->next, args);
		if (result != 0)
			return (-1);
		return (1);
	}
	else
	{
		result = process_command(cmd, args);
		if (result < 0)
			return (-1);
	}
	return (0);
}

/**
 * @brief Processes a chain of commands
 * @param cmd_head The head of the command list
 * @param env Array of environment variables
 * @param last_exit Pointer to the last exit status
 * @param tokens Pointer to the token list for cleanup during exit
 * @return 0 on success, -1 on error
 * @note Iterates through the command list and processes each command
 */
int	process_command_chain(t_command *cmd_head, char ***env, int *last_exit,
		t_token *tokens)
{
	t_command				*cmd;
	t_process_command_args	args;
	t_execution_context		exec_context;
	int						result;

	cmd = cmd_head;
	exec_context.last_exit = last_exit;
	exec_context.tokens = tokens;
	init_command_args(&args, cmd_head, env, &exec_context);
	while (cmd)
	{
		result = process_single_command(cmd, &args);
		if (result == -1)
			return (-1);
		if (result == 1)
			break ;
		cmd = cmd->next;
	}
	return (0);
}
