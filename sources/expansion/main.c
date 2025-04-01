/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:50:10 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/31 22:11:04 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static char	*init_expansion(char *str, int *params)
{
	char	*result;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	params[0] = 0;
	params[1] = 0;
	return (result);
}

static void	handle_char(char *str, char **result, int *i, int *in_quotes)
{
	char	*temp;

	if (str[*i] == '\'')
		*in_quotes = !(*in_quotes);
	temp = *result;
	*result = ft_strjoin_char(*result, str[*i]);
	free(temp);
	(*i)++;
}

char	*expand_variables(char *str, char **env, int last_exit)
{
	t_expansion_ctx	ctx;
	char			*result;
	int				params[2];

	ctx.env = env;
	ctx.last_exit = last_exit;
	result = init_expansion(str, params);
	if (!result)
		return (NULL);
	while (str[params[0]])
	{
		if (str[params[0]] == '$' && !params[1])
			result = process_var_expansion(result, str, &params[0], &ctx);
		else
			handle_char(str, &result, &params[0], &params[1]);
	}
	return (result);
}
