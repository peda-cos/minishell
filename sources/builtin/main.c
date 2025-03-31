/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:22:32 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/29 20:05:54 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

// TODO: Faz sentido ficar aqui se apenas é utilizado em sources/main.c?
// Nao deveria ser um método static do arquivo main.c?
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

// TODO: faz sentido esse método ficar aqui??
// Não deveria ser um método em arquivo utils.c 
// ja que é usado pelo builtin export
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

// TODO: faz sentido o nome desse arquivo ser main.c?
// Nao deveria ser env.c? ja que fornece um dos builtins necessários!
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
