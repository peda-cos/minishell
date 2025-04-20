/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/19 22:49:06 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

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
	while (tmp_content->next)
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

t_token_content	*new_token_content(char *value, int in_single_quotes)
{
	t_token_content	*content;

	if (!value)
		return (NULL);
	content = malloc(sizeof(t_token_content));
	content->next = NULL;
	content->value = value;
	content->in_single_quotes = in_single_quotes;
	return (content);
}

char	*get_token_content_value(t_content_params *params)
{
	t_token_content	*content;
	char			*temp_str;
	char			*value_str;
	char			*value_expanded;

	if (!params->content)
		return (NULL);
	value_str = ft_strdup("");
	content = params->content;
	while (content)
	{
		temp_str = value_str;
		if (content->in_single_quotes)
			value_expanded = ft_strdup(content->value);
		else
			value_expanded = expand_variable(content->value,
					params->envs, *params->last_exit_code);
		value_str = ft_strjoin(value_str, value_expanded);
		free(value_expanded);
		free(temp_str);
		content = content->next;
	}
	return (value_str);
}
