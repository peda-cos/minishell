/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:50:10 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/27 15:08:41 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static char	*extract_var_name(const char *str, int *index)
{
	int		start;

	start = *index;
	while (str[*index] && (ft_isalnum(str[*index]) || str[*index] == '_'))
		(*index)++;
	return (ft_substr(str, start, *index - start));
}

static char	*extract_env_value(
	char *str, char **envs, int last_exit, int *index)
{
	char	*var;
	char	*value;
	int		in_brackets_entered;

	value = get_special_variable(str, index, envs, last_exit);
	if (value)
	{
		(*index)++;
		return (value);
	}
	in_brackets_entered = str[*index] == BRACKET_OPEN_CHR;
	if (in_brackets_entered)
		var = extract_var_name_in_brackets_entered(str, index);
	else
		var = extract_var_name(str, index);
	if (in_brackets_entered && !var)
		return (NULL);
	if (!var)
		return (ft_strdup(""));
	value = get_env_value(var, envs);
	free(var);
	if (!value)
		return (ft_strdup(""));
	return (value);
}

static char	*extract_result(char *partial, char *result)
{
	char	*partial_temp;
	char	*result_temp;

	if (!partial)
		return (result);
	partial_temp = partial;
	result_temp = result;
	result = ft_strjoin(result, partial_temp);
	free(result_temp);
	free(partial_temp);
	return (result);
}

static void	process_expansion(char **result, char **value,
	int *has_error_flag_control)
{
	if (!*value)
		*has_error_flag_control = TRUE;
	*result = extract_result(*value, *result);
}

char	*expand_variable(char *str,
	char **envs, int last_exit, int *has_error_flag_control)
{
	int		i;
	int		start;
	char	*value;
	char	*result;
	int		str_len;

	i = 0;
	if (!str)
		return (NULL);
	result = ft_strdup("");
	str_len = ft_strlen(str);
	while (str[i] && i < str_len)
	{
		start = i;
		while (str[i] && str[i] != '$')
			i++;
		result = extract_result(ft_substr(str, start, i - start), result);
		if (str[i] == '$')
		{
			i++;
			value = extract_env_value(str, envs, last_exit, &i);
			process_expansion(&result, &value, has_error_flag_control);
		}
	}
	return (result);
}
