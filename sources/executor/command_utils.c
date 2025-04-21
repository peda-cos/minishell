/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:12:34 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/21 15:29:16 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd,
			"history"));
}

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
		return (builtin_env(*arg->env));
	else if (!ft_strcmp(command, "exit"))
		return (builtin_exit(arg));
	else if (!ft_strcmp(command, "history"))
		return (builtin_history(command_with_args, arg->last_exit));
	return (1);
}

static int	is_valid_executable(char *path)
{
	struct stat	path_stat;
	char		*err_stat;

	if (!path)
		return (FALSE);
	if (stat(path, &path_stat) != 0)
	{
		err_stat = ft_strjoin("minishell: ", path);
		ft_putstr_fd(err_stat, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		free(err_stat);
		return (FALSE);
	}
	if ((path[0] == '/' || path[0] == '.') && access(path, X_OK) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(path, STDERR_FILENO);
			ft_putendl_fd(": Is a directory", STDERR_FILENO);
			return (FALSE);
		}
		return (TRUE);
	}
	return (FALSE);
}

int	execute_external(t_command *cmd, char **env)
{
	char	*cmd_path;
	char	*command;

	command = cmd->args[0];
	if (!cmd || !cmd->args || !command || !env)
		return (1);
	if (ft_strchr(command, '/') && !is_valid_executable(command))
		return (126);
	cmd_path = find_executable(command, env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		return (127);
	}
	execve(cmd_path, cmd->args, env);
	perror("execve");
	free(cmd_path);
	return (1);
}
