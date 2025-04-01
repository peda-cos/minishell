/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:00:10 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/31 22:12:43 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char	*get_env_value(char *key, char **env)
{
	int		i;
	int		key_len;
	char	*value;

	if (!key || !env)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
		{
			value = ft_strdup(env[i] + key_len + 1);
			return (value);
		}
		i++;
	}
	return (ft_strdup(""));
}

char	*handle_exit_status(int last_exit)
{
	return (ft_itoa(last_exit));
}

char	*handle_variable(char *str, int *i, char **env, int last_exit)
{
	int		var_start;
	int		var_end;
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (handle_exit_status(last_exit));
	}
	var_start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_end = *i;
	if (var_start == var_end)
		return (ft_strdup("$"));
	var_name = ft_substr(str, var_start, var_end - var_start);
	var_value = get_env_value(var_name, env);
	free(var_name);
	return (var_value);
}

char	*process_var_expansion(char *result, char *str, int *i,
		t_expansion_ctx *ctx)
{
	char	*temp;
	char	*var_value;

	var_value = handle_variable(str, i, ctx->env, ctx->last_exit);
	temp = result;
	result = ft_strjoin(result, var_value);
	free(temp);
	free(var_value);
	return (result);
}
