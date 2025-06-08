/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 07:22:32 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/08 07:24:52 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
 * @brief Displays a single environment variable with declare -x format
 * @param env_str Environment string (NAME=value or NAME)
 * @return void
 * @note Formats and prints variable with appropriate quoting
 */
static void	display_single_var(char *env_str)
{
	char	*equal_pos;
	char	*name;
	char	*value;

	equal_pos = ft_strchr(env_str, '=');
	if (equal_pos)
	{
		name = ft_substr(env_str, 0, equal_pos - env_str);
		value = ft_strdup(equal_pos + 1);
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(name, STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(value, STDOUT_FILENO);
		ft_putendl_fd("\"", STDOUT_FILENO);
		free(name);
		free(value);
	}
	else
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(env_str, STDOUT_FILENO);
	}
}

/**
 * @brief Creates a sorted copy of environment array
 * @param env Original environment array
 * @param count Number of environment variables
 * @return Newly allocated sorted array or NULL on failure
 * @note Caller must free the returned array
 */
static char	**create_sorted_env(char **env, int count)
{
	char	**sorted_env;
	int		i;

	sorted_env = malloc(sizeof(char *) * (count + 1));
	if (!sorted_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		sorted_env[i] = env[i];
		i++;
	}
	sorted_env[count] = NULL;
	sort_env_alphabetically(sorted_env, count);
	return (sorted_env);
}

/**
 * @brief Counts the number of environment variables
 * @param env Environment array
 * @return Number of environment variables
 * @note Helper function for array processing
 */
static int	count_env_vars(char **env)
{
	int	count;

	count = 0;
	while (env[count])
		count++;
	return (count);
}

/**
 * @brief Displays environment variables in bash format with declare -x
 * @param env The environment variable array
 * @return 0 on success, 1 on failure
 * @note Sorts variables alphabetically and formats with declare -x prefix
 */
int	display_bash_export(char **env)
{
	char	**sorted_env;
	int		count;
	int		i;

	count = count_env_vars(env);
	sorted_env = create_sorted_env(env, count);
	if (!sorted_env)
		return (1);
	i = 0;
	while (sorted_env[i])
	{
		display_single_var(sorted_env[i]);
		i++;
	}
	free(sorted_env);
	return (0);
}
