/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:16:04 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/28 21:07:43 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	update_env_variable(char *key, char *value, char ***env)
{
	int		i;
	char	*temp;
	int		key_len;
	char	*new_value;

	key_len = ft_strlen(key);
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, key_len) == 0
			&& (*env)[i][key_len] == '=')
		{
			temp = ft_strjoin(key, "=");
			new_value = ft_strjoin(temp, value);
			free(temp);
			free((*env)[i]);
			(*env)[i] = new_value;
			return ;
		}
		i++;
	}
}

void	update_pwd_env(char *old_pwd, char ***env)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("cd: getcwd");
		return ;
	}
	update_env_variable("PWD", new_pwd, env);
	update_env_variable("OLDPWD", old_pwd, env);
	free(new_pwd);
}

int	change_to_previous(char ***env, char *old_pwd)
{
	char	*prev_pwd;

	prev_pwd = get_env_value("OLDPWD", *env, NULL);
	if (!prev_pwd)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
		return (1);
	}
	if (chdir(prev_pwd) != 0)
	{
		perror("cd");
		free(prev_pwd);
		return (1);
	}
	ft_putendl_fd(prev_pwd, STDOUT_FILENO);
	update_pwd_env(old_pwd, env);
	free(prev_pwd);
	return (0);
}

int	change_to_home(char ***env, char *old_pwd)
{
	char	*home;

	home = get_env_value("HOME", *env, NULL);
	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		return (1);
	}
	if (chdir(home) != 0)
	{
		perror("cd");
		free(home);
		return (1);
	}
	update_pwd_env(old_pwd, env);
	free(home);
	return (0);
}

int	change_directory(char *dir, char ***env, char *old_pwd)
{
	if (chdir(dir) != 0)
	{
		perror("cd");
		return (1);
	}
	update_pwd_env(old_pwd, env);
	return (0);
}
