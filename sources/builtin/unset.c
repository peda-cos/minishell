/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:18:12 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/25 14:28:30 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static void	remove_env_entry(char ***env, int idx)
{
	free((*env)[idx]);
	while ((*env)[idx])
	{
		(*env)[idx] = (*env)[idx + 1];
		idx++;
	}
}

int	builtin_unset(char **args, char ***env)
{
	int	idx;
	int	i;

	if (!args[1])
		return (0);
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
