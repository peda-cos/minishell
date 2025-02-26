/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:01:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/02/26 15:14:04 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

static int	exec_builtin(t_command *cmd, char **envp)
{
	int	i;

	(void)envp;
	if (!ft_strcmp(cmd->args[0], "echo"))
	{
		i = 1;
		while (cmd->args[i])
		{
			printf("%s", cmd->args[i]);
			if (cmd->args[i + 1])
				printf(" ");
			i++;
		}
		printf("\n");
		return (0);
	}
	if (!ft_strcmp(cmd->args[0], "exit"))
		exit(0);
	return (0);
}

void	execute_command(t_command *cmd, char **envp)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (is_builtin(cmd->args[0]))
	{
		exec_builtin(cmd, envp);
		return ;
	}
	cmd_path = find_executable(cmd->args[0], envp);
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
		if (execve(cmd_path, cmd->args, envp) < 0)
		{
			perror("execve");
			exit(1);
		}
	}
	waitpid(pid, &status, 0);
	free(cmd_path);
}
