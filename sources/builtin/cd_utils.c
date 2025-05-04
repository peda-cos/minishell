/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:16:04 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 19:21:18 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
	* @brief Updates the value of an environment variable
	* @param key The name of the environment variable to update
	* @param value The new value to set for the environment variable
	* @param env A pointer to the environment variable array
	* @return void
	* @note Iterates through the environment variable array
	*       and updates the value if the key is found
 */
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

/**
	* @brief Updates the PWD and OLDPWD environment variables
	* @param old_pwd The previous working directory to set as OLDPWD
	* @param env A pointer to the environment variable array
	* @return void
	* @note Uses getcwd to retrieve the current working directory
	*/
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

/**
	* @brief Changes the current working directory to the previous one
	* @param env A pointer to the environment variable array
	* @param old_pwd The previous working directory to set as OLDPWD
	* @return 0 on success, 1 on failure
	* @note Uses get_env_value to retrieve 
	* the value of OLDPWD from the environment
	* If OLDPWD is not set, prints an error message and returns 1
	*/
int	change_to_previous(char ***env, char *old_pwd)
{
	char	*prev_pwd;

	prev_pwd = get_env_value("OLDPWD", *env);
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

/**
	* @brief Changes the current working directory to the home directory
	* @param env A pointer to the environment variable array
	* @param old_pwd The previous working directory to set as OLDPWD
	* @return 0 on success, 1 on failure
	* @note Uses get_env_value to retrieve
	* the value of HOME from the environment
	* If HOME is not set, prints an error message and returns 1
	*/
int	change_to_home(char ***env, char *old_pwd)
{
	char	*home;

	home = get_env_value("HOME", *env);
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

/**
	* @brief Changes the current working directory to the specified directory
	* @param dir The directory to change to
	* @param env A pointer to the environment variable array
	* @param old_pwd The previous working directory to set as OLDPWD
	* @return 0 on success, 1 on failure
	* @note Uses chdir to change the current working directory
	*/
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
