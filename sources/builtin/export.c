/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:20:51 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/25 14:28:14 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

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

static int	update_existing_entry(char ***env, char *new_entry, int idx)
{
	free((*env)[idx]);
	(*env)[idx] = new_entry;
	return (0);
}

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

static int	process_export_arg(char ***env, char *arg)
{
	int		idx;
	char	*new_entry;

	new_entry = ft_strdup(arg);
	if (!new_entry)
		return (1);
	idx = find_env_index(arg, *env);
	if (idx >= 0)
		return (update_existing_entry(env, new_entry, idx));
	else
		return (add_new_entry(env, new_entry));
}

int	builtin_export(char **args, char ***env)
{
	int	i;
	int	status;

	if (!*env)
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
