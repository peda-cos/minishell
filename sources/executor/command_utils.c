/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:12:34 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/13 19:56:09 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
	* @brief Validates if the given path is executable
	* @param path The path to validate
	* @return 0 if valid, 126 if not executable, 127 if not found
	* @note Uses stat to check if the file exists 
	* and access to check if it is executable
 */
static int	validates_executable(char *path)
{
	struct stat	path_stat;
	char		*err_stat;

	if (!path)
		return (126);
	if (stat(path, &path_stat) != 0)
	{
		err_stat = ft_strjoin("minishell: ", path);
		ft_putstr_fd(err_stat, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		free(err_stat);
		return (127);
	}
	if ((path[0] == '/' || path[0] == '.') && access(path, X_OK) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(path, STDERR_FILENO);
			ft_putendl_fd(": Is a directory", STDERR_FILENO);
			return (126);
		}
		return (0);
	}
	return (126);
}

/**
 * @brief Prints an error message for an invalid command
 * @param command The command that caused the error
 * @param err_msg The error message to print
 * @return 2 if the command is ".", 127 otherwise
 * @note Prints the error message to standard error output
 */
static int	invalid_command(char *command, char *err_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(err_msg, STDERR_FILENO);
	if (command[0] == '.' && command[1] == NULL_CHR)
		return (2);
	return (127);
}

/**
 * @brief Checks if the command is a built-in command
 * @param cmd The command to check
 * @return 1 if it is a built-in command, 0 otherwise
 * @note Compares the command with known built-in commands
 */
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd,
			"history"));
}

/**
 * @brief Executes a built-in command
 * @param arg The command arguments and environment variables
 * @return 0 on success, 1 on failure
 * @note Calls the appropriate built-in
	* function based on the command name
 */
int	execute_builtin(t_process_command_args *arg)
{
	char	*command;
	char	**command_with_args;

	command = arg->cmd->args[0];
	command_with_args = arg->cmd->args;
	if (!arg->cmd || !command_with_args
		|| !command || !arg->env || !arg->last_exit)
		return (1);
	if (!ft_strcmp(command, "echo"))
		return (builtin_echo(command_with_args, arg->last_exit));
	else if (!ft_strcmp(command, "cd"))
		return (builtin_cd(command_with_args, arg->env));
	else if (!ft_strcmp(command, "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(command, "export"))
		return (builtin_export(command_with_args, arg->env));
	else if (!ft_strcmp(command, "unset"))
		return (builtin_unset(command_with_args, arg->env));
	else if (!ft_strcmp(command, "env"))
		return (builtin_env(command_with_args, *arg->env));
	else if (!ft_strcmp(command, "history"))
		return (builtin_history(command_with_args, arg->last_exit));
	else if (!ft_strcmp(command, "exit"))
		builtin_exit(arg);
	return (1);
}

/**
 * @brief Executes an external command
 * @param cmd The command to execute
 * @param env The environment variables
 * @return 0 on success, 1 on failure
 * @note Uses execve to execute the command
	* with the given arguments and environment
 */
int	execute_external(t_command *cmd, char **env)
{
	char	*cmd_path;
	char	*command;
	int		has_signal_validation_executable;

	command = cmd->args[0];
	if (!cmd || !cmd->args || !command || !env)
		return (1);
	if (ft_strchr(command, '/'))
	{
		has_signal_validation_executable = validates_executable(command);
		if (has_signal_validation_executable > 0)
			return (has_signal_validation_executable);
	}
	if (ft_strcmp(command, "..") == 0)
		return (invalid_command(command, "command not found"));
	if (ft_strcmp(command, ".") == 0)
		return (invalid_command(command, "file argument required"));
	cmd_path = find_executable(command, env);
	if (!cmd_path)
		return (invalid_command(command, "command not found"));
	execve(cmd_path, cmd->args, env);
	perror("execve");
	free(cmd_path);
	return (1);
}
