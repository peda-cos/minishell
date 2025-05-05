/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:20:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 18:44:04 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
 * @brief Handles different cd command variants based on arguments
 * @param args The command-line arguments array
 * @param env Triple pointer to the environment variables array
 * @param old_pwd The previous working directory to set as OLDPWD
 * @return 0 on success, 1 on failure
 * @note Routes to appropriate handler functions for different cd cases
 */
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

/**
 * @brief Changes the current working directory to the specified directory
 * @param args The command-line arguments
 * @param env A pointer to the environment variables array
 * @return 0 on success, 1 on failure
 * @note Handles too many arguments with error message
 *       Gets current working directory as old_pwd
 */
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
