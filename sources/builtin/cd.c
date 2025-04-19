/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:20:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/19 16:52:37 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	exec_cd(char **args, char ***env, char *old_pwd)
{
	if (!args[1]
		|| ft_strcmp(args[1], "--") == 0
		|| ft_strcmp(args[1], "~") == 0)
		return (change_to_home(env, old_pwd));
	else if (ft_strcmp(args[1], "-") == 0)
		return (change_to_previous(env, old_pwd));
	return (change_directory(args[1], env, old_pwd));
}

int	builtin_cd(char **args, char ***env)
{
	char	*old_pwd;
	int		status;

	if (!args || !env || !*env)
		return (1);
	if (args[2] != NULL)
	{
		ft_putstr_fd("Minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		perror("cd: getcwd");
		return (1);
	}
	status = exec_cd(args, env, old_pwd);
	free(old_pwd);
	return (status);
}
