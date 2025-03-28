/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:18:12 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/28 01:00:55 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static void	remove_env_entry(char ***env, int idx)
{
	// printf("Removing entry\n");
	free((*env)[idx]);
	while ((*env)[idx])
	{
		(*env)[idx] = (*env)[idx + 1];
		idx++;
	}
}

int	builtin_unset(char **args, char ***env)
{
	// printf("Unsetting\n");
	int	idx;
	int	i;

	if (!args ||	!*env)
		return (1);
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

// int main(int argc, char *argv, char **env)
// {
// 	char *args[] = { "unset", "HELLO", "HELLO2", NULL };
// 	char **env_copy = copy_env(env);
// 	builtin_unset(args, &env_copy);
// 	while (*env_copy)
// 	{
// 		free(*env_copy);
// 		env_copy++;
// 	}
// 	return (0);
// }
