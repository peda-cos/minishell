/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:22:32 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/28 00:57:15 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

// TODO: Faz sentido ficar aqui se apenas é utilizado ein sources/main.c?
char	**copy_env(char **envp)
{
	char	**env;
	int		i;
	int		count;

	if (!envp ||	!*envp)
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
		printf("Copying env[%d]\n", i);
		env[i] = ft_strdup(envp[i]);
		i++;
	}
	printf("Copied env\n");
	env[i] = NULL;
	return (env);
}

int	find_env_index(const char *key, char **env)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	builtin_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (1);
	while (env[i])
	{
		ft_putendl_fd(env[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}
