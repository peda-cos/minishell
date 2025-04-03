/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:20:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/02 22:55:50 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static void	update_pwd_env(char *old_pwd, char ***env)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("cd: getcwd");
		return ;
	}
	update_env_variable("PWD=", new_pwd, env);
	update_env_variable("OLDPWD=", old_pwd, env);
	free(new_pwd);
}

static int	change_to_home(char ***env, char *old_pwd)
{
	char	*home;

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
		free(home);
		return (1);
	}
	update_pwd_env(old_pwd, env);
	free(home);
	return (0);
}

static int	change_directory(char *dir, char ***env, char *old_pwd)
{
	if (chdir(dir) != 0)
	{
		perror("cd");
		free(old_pwd);
		return (1);
	}
	update_pwd_env(old_pwd, env);
	return (0);
}

int	builtin_cd(char **args, char ***env)
{
	char	*old_pwd;
	int		status;

	if (!args || !env || !*env)
		return (1);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		perror("cd: getcwd");
		return (1);
	}
	if (!args[1])
		status = change_to_home(env, old_pwd);
	else
		status = change_directory(args[1], env, old_pwd);
	if (status == 0)
		free(old_pwd);
	return (status);
}
