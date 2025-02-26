/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:01:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/02/26 16:18:35 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
		!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") ||
		!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
		!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

static int	exec_builtin(t_command *cmd, char **env)
{
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (builtin_echo(cmd->args));
	else if (!ft_strcmp(cmd->args[0], "cd"))
		return (builtin_cd(cmd->args));
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(cmd->args[0], "export"))
		return (builtin_export(cmd->args, &env));
	else if (!ft_strcmp(cmd->args[0], "unset"))
		return (builtin_unset(cmd->args, &env));
	else if (!ft_strcmp(cmd->args[0], "env"))
		return (builtin_env(env));
	else if (!ft_strcmp(cmd->args[0], "exit"))
		return (builtin_exit(cmd->args));
	return (0);
}

void	execute_command(t_command *cmd, char **env)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (is_builtin(cmd->args[0]))
	{
		exec_builtin(cmd, env);
		return ;
	}
	cmd_path = find_executable(cmd->args[0], env);
	if (!cmd_path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		return ;
	}
	if (pid == 0)
	{
		if (execve(cmd_path, cmd->args, env) < 0)
		{
			perror("execve");
			exit(1);
		}
	}
	waitpid(pid, &status, 0);
	free(cmd_path);
}
