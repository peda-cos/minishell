/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:01:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/05 16:25:55 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Executes a command and handles redirection
 * @param cmd The command structure to be executed
 * @param env Array of environment variables
 * @param last_exit Pointer to the last exit status
 * @param tokens Pointer to the token list for cleanup during exit
 * @return void
 * @note Sets up signal handling and
 * restores standard file descriptors after execution
 */
void	execute_command(t_command *cmd, char ***env, int *last_exit,
		t_token *tokens)
{
	int	stdin_backup;
	int	pipeline_exists;

	stdin_backup = setup_command_execution(cmd, *env, last_exit);
	if (stdin_backup < 0)
		return ;
	pipeline_exists = has_pipeline(cmd);
	process_command_chain(cmd, env, last_exit, tokens);
	cleanup_command_execution(pipeline_exists, stdin_backup);
}
