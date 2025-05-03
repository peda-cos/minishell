/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/02 20:44:47 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

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

t_token	*new_token(t_token_content *content, t_token_type type)
{
	t_token			*token;
	t_token_content	*tmp_content;

	token = malloc(sizeof(t_token));
	if (!token || !content)
		return (NULL);
	token->length = 0;
	token->prev = NULL;
	token->next = NULL;
	token->type = type;
	token->content = content;
	tmp_content = content;
	while (tmp_content)
	{
		token->length += ft_strlen(tmp_content->value);
		tmp_content = tmp_content->next;
	}
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
	new->prev = tmp;
	tmp->next = new;
}

t_token	*get_last_token(t_token **tokens)
{
	t_token	*tmp;

	if (!*tokens)
		return (NULL);
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}
