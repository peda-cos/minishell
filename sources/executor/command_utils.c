/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:12:34 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/25 08:16:22 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"));
}

int	execute_builtin(t_command *cmd, char ***env, int *last_exit)
{
	if (!cmd || !cmd->args || !cmd->args[0] || !env || !last_exit)
		return (1);
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (builtin_echo(cmd->args, last_exit));
	else if (!ft_strcmp(cmd->args[0], "cd"))
		return (builtin_cd(cmd->args));
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(cmd->args[0], "export"))
		return (builtin_export(cmd->args, env));
	else if (!ft_strcmp(cmd->args[0], "unset"))
		return (builtin_unset(cmd->args, env));
	else if (!ft_strcmp(cmd->args[0], "env"))
		return (builtin_env(*env));
	else if (!ft_strcmp(cmd->args[0], "exit"))
		return (builtin_exit(cmd->args));
	return (1);
}

int	execute_external(t_command *cmd, char **env)
{
	char	*cmd_path;

	if (!cmd || !cmd->args || !cmd->args[0] || !env)
		return (1);
	cmd_path = find_executable(cmd->args[0], env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		return (127);
	}
	execve(cmd_path, cmd->args, env);
	perror("execve");
	free(cmd_path);
	return (1);
}
