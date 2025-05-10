/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:01:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/09 21:21:45 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Prepares for command execution by setting up necessary file descriptors
 * @param cmd The command structure to be executed
 * @param env Array of environment variables
 * @param last_exit Pointer to the last exit status
 * @return Backup file descriptor for stdin, or -1 on error
 * @note Validates inputs and creates a backup of stdin for later restoration
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
	manager_file_descriptors(ADD, backup_fd);
	return (backup_fd);
}

/**
 * @brief Checks if the command has a pipeline (multiple commands)
 * @param cmd The command structure to check
 * @return 1 if a pipeline exists, 0 otherwise
 * @note Checks if the command has a next command in the chain
 */
static int	has_pipeline(t_command *cmd)
{
	if (cmd && cmd->next)
		return (1);
	return (0);
}

/**
 * @brief Processes a single command in the command chain
 * @param cmd The command structure to be executed
 * @param env Array of environment variables
 * @param last_exit Pointer to the last exit status
 * @param tokens Pointer to the token list for cleanup during exit
 * @return 0 on success, -1 on error
 * @note Handles the execution of a single command and its arguments
 */
static int	process_command_chain(t_command *cmd_head,
	char ***env, int *last_exit, t_token *tokens)
{
	t_command				*cmd;
	t_process_command_args	args;
	int						result;

	cmd = cmd_head;
	args.env = env;
	args.head = cmd_head;
	args.tokens = tokens;
	args.last_exit = last_exit;
	while (cmd)
	{
		result = process_command(cmd, &args);
		if (result < 0)
			return (-1);
		cmd = cmd->next;
	}
	return (0);
}

/**
 * @brief Cleans up after command execution, restoring file descriptors
 * @param pipeline_exists Flag indicating if a pipeline exists
 * @param stdin_backup Backup file descriptor for stdin
 * @return void
 * @note Restores the original stdin and closes the backup file descriptor
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
