/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:22:32 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 18:44:40 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
	* @brief Copies the environment variables into a new array
	* @param envp The original environment variable array
	* @return A new array of strings containing the copied environment variables
	* @note Allocates memory for the new array and each string
	*       Returns NULL if memory allocation fails
	*/
char	**copy_env(char **envp)
{
	int		i;
	int		count;
	char	**env;

	if (!envp || !*envp)
		return (NULL);
	count = 0;
	while (envp[count])
		count++;
	env = malloc(sizeof(char *) * (count + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		i++;
	}
	env[i] = NULL;
	return (env);
}

/**
	* @brief Finds the index of an environment variable in the array
	* @param key The name of the environment variable to search for
	* @param env The environment variable array
	* @return The index of the variable in the array, or -1 if not found
	* @note Allocates memory for the key substring and frees it after use
	*/
int	find_env_index(char *key, char **env)
{
	int		i;
	char	*env_key;
	char	*equal_chr_ptr;

	i = 0;
	if (!key || !env)
		return (-1);
	while (env[i])
	{
		equal_chr_ptr = ft_strchr(env[i], '=');
		env_key = ft_substr(env[i], 0, equal_chr_ptr - env[i]);
		if (ft_strcmp(key, env_key) == 0)
		{
			free(env_key);
			return (i);
		}
		free(env_key);
		i++;
	}
	return (-1);
}

/**
	* @brief Prints the environment variables to standard output
	* @param args The command-line arguments
	* @param env The environment variable array
	* @return 0 on success, 1 on failure
	* @note Prints each environment variable on a new line
	*       Returns 127 if arguments are provided
	*/
int	builtin_env(char **args, char **env)
{
	int	i;

	i = 0;
	if (!args || !env)
		return (1);
	if (args[1])
	{
		ft_putendl_fd(
			"Minishell: Env with arguments not implemented", STDERR_FILENO);
		return (127);
	}
	while (env[i])
	{
		ft_putendl_fd(env[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}
