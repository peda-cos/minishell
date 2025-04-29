/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:00:10 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/28 21:07:40 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char	*get_env_value(char *key, char **envs, t_token_content *content)
{
	int		i;
	int		key_len;
	char	*value;

	if (!key || !envs)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (envs[i])
	{
		if (ft_strncmp(envs[i], key, key_len) == 0 && envs[i][key_len] == '=')
		{
			value = ft_strdup(envs[i] + key_len + 1);
			if (content)
				content->was_expanded = TRUE;
			return (value);
		}
		i++;
	}
	return (ft_strdup(""));
}

char	*get_underscore_arg_value(char **envs)
{
	char	*value;

	value = get_env_value("_", envs, NULL);
	if (!value)
		return (getcwd(NULL, 0));
	return (value);
}

void	set_underscore_arg_value(t_command *cmd, char ***envs)
{
	int		index;
	char	*last_arg_value;

	index = 0;
	while (cmd->args && cmd->args[index])
		index++;
	if (index > 0)
	{
		last_arg_value = cmd->args[index - 1];
		update_env_variable("_", last_arg_value, envs);
	}
}

char	*get_special_variable(char *str,
	int *index, char **envs, int last_exit)
{
	char	*value;

	value = NULL;
	if (ft_isspace(str[*index]))
		value = (ft_strdup("$ "));
	else if (str[*index] == '\0')
		value = (ft_strdup("$"));
	else if (str[*index] == '?')
		value = ft_itoa(last_exit);
	else if (str[*index] == '0')
		value = (ft_strdup("minishell"));
	else if (str[*index] == '_')
		value = (get_underscore_arg_value(envs));
	return (value);
}
