/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:01:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/22 18:44:06 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static char	*join_path(char *dir, char *cmd)
{
	char	*temp;
	char	*full;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full = ft_strjoin(temp, cmd);
	free(temp);
	return (full);
}

static int	handle_heredoc(char *delim)
{
	int		pipefd[2];
	char	*line;
	char	*buffer;
	char	*temp;

	buffer = ft_strdup("");
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		free(buffer);
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, delim))
		{
			free(line);
			break ;
		}
		{
			temp = buffer;
			buffer = ft_strjoin(buffer, line);
			free(temp);
			temp = buffer;
			buffer = ft_strjoin(buffer, "\n");
			free(temp);
		}
		free(line);
	}
	write(pipefd[1], buffer, ft_strlen(buffer));
	close(pipefd[1]);
	free(buffer);
	return (pipefd[0]);
}

char	*find_executable(char *cmd, char **env)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path_env = env[i] + 5;
			paths = ft_split(path_env, ':');
			if (!paths)
				return (NULL);
			i = 0;
			while (paths[i])
			{
				full_path = join_path(paths[i], cmd);
				if (full_path && access(full_path, X_OK) == 0)
				{
					free_split(paths);
					return (full_path);
				}
				if (full_path)
					free(full_path);
				i++;
			}
			free_split(paths);
			break ;
		}
		i++;
	}
	return (NULL);
}

void	execute_command(t_command *cmd, char **env, int *last_exit)
{
	t_command	*cur;
	int			pipefd[2];
	pid_t		pid;
	int			status;
	int			in_fd;
	int			fd;
	char		*cmd_path;
	int			hd_fd;

	cur = cmd;
	in_fd = 0;
	while (cur)
	{
		if (cur->next)
		{
			if (pipe(pipefd) < 0)
			{
				perror("pipe");
				return ;
			}
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			return ;
		}
		if (pid == 0)
		{
			if (cur->heredoc_delim)
			{
				hd_fd = handle_heredoc(cur->heredoc_delim);
				if (hd_fd < 0)
					exit(1);
				dup2(hd_fd, STDIN_FILENO);
				close(hd_fd);
			}
			if (cur->input_file)
			{
				fd = open(cur->input_file, O_RDONLY);
				if (fd < 0)
				{
					perror(cur->input_file);
					exit(1);
				}
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
			if (cur->output_file)
			{
				if (cur->append)
					fd = open(cur->output_file, O_WRONLY | O_CREAT | O_APPEND,
							0644);
				else
					fd = open(cur->output_file, O_WRONLY | O_CREAT | O_TRUNC,
							0644);
				if (fd < 0)
				{
					perror(cur->output_file);
					exit(1);
				}
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			if (cur->next)
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
			}
			if (cur->argc > 0)
			{
				if (!ft_strcmp(cur->args[0], "echo"))
					exit(builtin_echo(cur->args, last_exit));
				else if (!ft_strcmp(cur->args[0], "cd"))
					exit(builtin_cd(cur->args));
				else if (!ft_strcmp(cur->args[0], "pwd"))
					exit(builtin_pwd());
				else if (!ft_strcmp(cur->args[0], "export"))
					exit(builtin_export(cur->args, &env));
				else if (!ft_strcmp(cur->args[0], "unset"))
					exit(builtin_unset(cur->args, &env));
				else if (!ft_strcmp(cur->args[0], "env"))
					exit(builtin_env(env));
				else if (!ft_strcmp(cur->args[0], "exit"))
					exit(builtin_exit(cur->args));
				cmd_path = find_executable(cur->args[0], env);
				if (!cmd_path)
				{
					fprintf(stderr, "minishell: %s: command not found\n",
						cur->args[0]);
					exit(127);
				}
				execve(cmd_path, cur->args, env);
				perror("execve");
				free(cmd_path);
				exit(1);
			}
		}
		if (cur->next)
		{
			close(pipefd[1]);
			in_fd = pipefd[0];
			dup2(in_fd, STDIN_FILENO);
		}
		waitpid(pid, &status, 0);
		*last_exit = WEXITSTATUS(status);
		cur = cur->next;
	}
	dup2(0, STDIN_FILENO);
}
