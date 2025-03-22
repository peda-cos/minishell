/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/03/22 15:11:32 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token *new)
{
	t_token	*tmp;

	if (!*tokens)
	{
		*tokens = new;
		return ;
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	is_metachar(char c)
{
	return (c == PIPE_CHR
		|| c == REDIRECT_IN_CHR
		|| c == REDIRECT_OUT_CHR);
}

int	is_quote_delimiter(char *str, int *index, char delimiter)
{
	char	chr;
	int		is_quote;

	chr = str[*index];
	is_quote = (chr == delimiter);
	if (*index == 0 && is_quote)
		return (TRUE);
	else if (is_quote && str[*index - 1] != BACKSLASH_CHR)
		return (TRUE);
	return (FALSE);
}
