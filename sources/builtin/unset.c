/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:18:12 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/03 15:47:49 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
 * @brief Removes an environment variable entry at the specified index
 * @param env Triple pointer to the environment variables array
 * @param idx The index of the entry to remove
 * @return void
 * @note Frees the memory of the removed
	* entry and shifts all remaining entries
 */
static void	remove_env_entry(char ***env, int idx)
{
	free((*env)[idx]);
	while ((*env)[idx])
	{
		(*env)[idx] = (*env)[idx + 1];
		idx++;
	}
}

/**
 * @brief Removes environment variables from the environment array
 * @param args The command-line arguments
 * @param env A pointer to the environment variable array
 * @return 0 on success, 1 on failure
 * @note Iterates through each argument and
	* removes the corresponding entry
 */
int	builtin_unset(char **args, char ***env)
{
	int	idx;
	int	i;

	if (!args || !*args || !env || !*env)
		return (1);
	if (!args[1])
		return (0);
	if (args[1][0] == DASH_CHR)
	{
		print_invalid_option("unset", args[1], UNSET_OPTIONS_CHARS);
		return (2);
	}
	i = 1;
	while (args[i])
	{
		idx = find_env_index(args[i], *env);
		if (idx >= 0)
			remove_env_entry(env, idx);
		i++;
	}
	return (0);
}
