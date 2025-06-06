/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_setup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:17:31 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/05 16:22:12 by peda-cos         ###   ########.fr       */
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
int	setup_command_execution(t_command *cmd, char **env, int *last_exit)
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
int	has_pipeline(t_command *cmd)
{
	if (cmd && cmd->next)
		return (1);
	return (0);
}

/**
 * @brief Cleans up after command execution, restoring file descriptors
 * @param pipeline_exists Flag indicating if a pipeline exists
 * @param stdin_backup Backup file descriptor for stdin
 * @return void
 * @note Restores the original stdin and closes the backup file descriptor
 */
void	cleanup_command_execution(int pipeline_exists, int stdin_backup)
{
	int	status;

	if (pipeline_exists)
	{
		while (waitpid(-1, &status, 0) > 0)
		{
		}
	}
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
}
