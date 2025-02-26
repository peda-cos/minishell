/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:22:32 by peda-cos          #+#    #+#             */
/*   Updated: 2025/02/26 18:03:05 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **args, int *last_exit)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	(void)last_exit;
	if (args[1] && !ft_strcmp(args[1], "-n"))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	builtin_cd(char **args)
{
	if (!args[1])
	{
		fprintf(stderr, "cd: missing argument\n");
		return (1);
	}
	if (chdir(args[1]) != 0)
		perror("cd");
	return (0);
}

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

static int	find_env_index(const char *key, char **env)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	builtin_export(char **args, char ***env)
{
	int		i;
	int		idx;
	char	*new_entry;
	char	**new_env;
	int		count;
	int		j;

	if (!*env)
		return (1);
	if (!args[1])
	{
		i = 0;
		while ((*env)[i])
		{
			printf("%s\n", (*env)[i]);
			i++;
		}
		return (0);
	}
	i = 1;
	while (args[i])
	{
		new_entry = ft_strdup(args[i]);
		if (!new_entry)
			return (1);
		idx = find_env_index(args[i], *env);
		if (idx >= 0)
		{
			free((*env)[idx]);
			(*env)[idx] = new_entry;
		}
		else
		{
			count = 0;
			while ((*env)[count])
				count++;
			new_env = malloc(sizeof(char *) * (count + 2));
			if (!new_env)
			{
				free(new_entry);
				return (1);
			}
			{
				j = 0;
				while (j < count)
				{
					new_env[j] = (*env)[j];
					j++;
				}
				new_env[j] = new_entry;
				new_env[j + 1] = NULL;
			}
			free(*env);
			*env = new_env;
		}
		i++;
	}
	return (0);
}

int	builtin_unset(char **args, char ***env)
{
	int	idx;
	int	i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		idx = find_env_index(args[i], *env);
		if (idx >= 0)
		{
			free((*env)[idx]);
			while ((*env)[idx])
			{
				(*env)[idx] = (*env)[idx + 1];
				idx++;
			}
		}
		i++;
	}
	return (0);
}

int	builtin_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (1);
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (0);
}

int	builtin_exit(char **args)
{
	int	exit_code;

	exit_code = 0;
	if (args[1])
		exit_code = ft_atoi(args[1]);
	exit(exit_code);
	return (0);
}
