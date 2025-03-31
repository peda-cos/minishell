/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:20:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/31 20:43:03 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static char	*get_env_value(char *key, char **env)
{
	int	i;
	int	key_len;

	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return (env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

static void	update_env_variable(char *key, char *value, char ***env)
{
	int		i;
	int		key_len;
	char	*new_value;

	key_len = ft_strlen(key);
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, key_len) == 0
			&& (*env)[i][key_len] == '=')
		{
			new_value = ft_strjoin(key, value);
			free((*env)[i]);
			(*env)[i] = new_value;
			return ;
		}
		i++;
	}
}

int	builtin_cd(char **args, char ***env)
{
	char	*home;
	char	*old_pwd;
	char	*new_pwd;

	if (!args || !env || !*env)
		return (1);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		perror("cd: getcwd");
		return (1);
	}
	if (!args[1])
	{
		home = get_env_value("HOME", *env);
		if (!home)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			free(old_pwd);
			return (1);
		}
		if (chdir(home) != 0)
		{
			perror("cd");
			free(old_pwd);
			return (1);
		}
	}
	else if (chdir(args[1]) != 0)
	{
		perror("cd");
		free(old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("cd: getcwd");
		free(old_pwd);
		return (1);
	}
	update_env_variable("PWD=", new_pwd, env);
	update_env_variable("OLDPWD=", old_pwd, env);
	free(old_pwd);
	free(new_pwd);
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
	ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	return (0);
}
