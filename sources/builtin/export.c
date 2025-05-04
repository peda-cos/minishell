/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:20:51 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 19:29:25 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
	* @brief Displays the environment variables
	* @param env The environment variable array
	* @return 0 on success
	* @note Iterates through each string
	* in the array and prints it to standard output
	*/
static int	display_environment(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_putendl_fd(env[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}

/**
	* @brief Finds the index of an environment variable in the array
	* @param key The name of the environment variable to search for
	* @param env The environment variable array
	* @return The index of the variable in the array, or -1 if not found
	* @note Allocates memory for the key substring and frees it after use
	*/
static int	update_existing_entry(char ***env, char *new_entry, int idx)
{
	free((*env)[idx]);
	(*env)[idx] = new_entry;
	return (0);
}

/**
	* @brief Adds a new entry to the environment variable array
	* @param env A pointer to the environment variable array
	* @param new_entry The new entry to add
	* @return 0 on success, 1 on failure
	* @note Allocates memory for the new array and each string
	*/
static int	add_new_entry(char ***env, char *new_entry)
{
	char	**new_env;
	int		count;
	int		j;

	count = 0;
	while ((*env)[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	j = 0;
	while (j < count)
	{
		new_env[j] = (*env)[j];
		j++;
	}
	new_env[j] = new_entry;
	new_env[j + 1] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}

/**
	* @brief Processes the export argument and
	* updates the environment variable array
	* @param env A pointer to the environment variable array
	* @param arg The argument to process
	* @return 0 on success, 1 on failure
	* @note Validates the key and updates or
	* adds the entry in the environment variable array
	*/
static int	process_export_arg(char ***env, char *arg)
{
	int		idx;
	char	*arg_key;
	char	*new_entry;
	char	*equal_chr_ptr;

	new_entry = ft_strdup(arg);
	if (!new_entry)
		return (1);
	equal_chr_ptr = ft_strchr(arg, '=');
	if (equal_chr_ptr)
		arg_key = ft_substr(arg, 0, equal_chr_ptr - arg);
	else
		arg_key = ft_strdup(arg);
	if (!is_valid_key(arg_key))
	{
		free(new_entry);
		free(arg_key);
		return (print_invalid_arg(arg));
	}
	idx = find_env_index(arg_key, *env);
	free(arg_key);
	if (idx >= 0)
		return (update_existing_entry(env, new_entry, idx));
	return (add_new_entry(env, new_entry));
}

/**
	* @brief Built-in export command implementation
	* @param args The command-line arguments
	* @param env A pointer to the environment variable array
	* @return 0 on success, 1 on failure
	* @note Processes each argument and updates the environment variable array
	*/
int	builtin_export(char **args, char ***env)
{
	int	i;
	int	status;

	if (! env || !*env || !args || !*args)
		return (1);
	if (!args[1])
		return (display_environment(*env));
	i = 1;
	status = 0;
	while (args[i] && status == 0)
	{
		status = process_export_arg(env, args[i]);
		i++;
	}
	return (status);
}
