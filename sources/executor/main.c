/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:01:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/21 03:12:58 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Validates inputs and creates a backup of standard input
 * @return File descriptor for the backup, or -1 on error/invalid input
 */
static int	setup_command_execution(t_command *cmd, char **env, int *last_exit)
{
	int	backup_fd;

	if (!cmd || !env || !last_exit)
		return (-1);
	backup_fd = dup(STDIN_FILENO);
	if (backup_fd < 0)
	{
		perror("dup");
		return (-1);
	}
	return (backup_fd);
}

/**
 * @brief Determines if the command chain contains a pipeline
 * @return 1 if pipeline exists, 0 otherwise
 */
static int	has_pipeline(t_command *cmd)
{
	if (cmd && cmd->next)
		return (1);
	return (0);
}

/**
 * @brief Iterates through and processes each command in the chain
 * @return 0 on success, -1 on failure
 */
static int	process_command_chain(t_command *cmd, char ***env, int *last_exit,
		t_token *tokens)
{
	int	result;

	while (cmd)
	{
		result = process_command(cmd, env, last_exit, tokens);
		if (result < 0)
			return (-1);
		cmd = cmd->next;
	}
	return (0);
}

/**
 * @brief Waits for child processes and restores stdin if needed
 */
static void	cleanup_command_execution(int pipeline_exists, int stdin_backup)
{
	int	status;

	if (pipeline_exists)
		while (waitpid(-1, &status, 0) > 0)
			dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
}

/**
 * @brief Executes a chain of commands with proper handling of pipelines
 *
 * This is the main coordination function that handles the entire execution
 * flow of commands, including pipeline handling and resource management.
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
