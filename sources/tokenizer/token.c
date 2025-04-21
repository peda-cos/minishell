/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/20 21:02:57 by jlacerda         ###   ########.fr       */
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

t_token_content	*new_token_content(char *value, int in_single_quotes)
{
	t_token_content	*content;

	if (!value)
		return (NULL);
	content = malloc(sizeof(t_token_content));
	if (!content)
	{
		free(value);
		return (NULL);
	}
	content->next = NULL;
	content->value = value;
	content->in_single_quotes = in_single_quotes;
	return (content);
}

static char	*get_env_value_expanded(char *current_value_str,
	t_token_content	*content, t_content_params *params)
{
	char			*result;
	char			*temp_str;
	char			*value_expanded;

	temp_str = current_value_str;
	if (content->in_single_quotes)
		value_expanded = ft_strdup(content->value);
	else
		value_expanded = expand_variable(content->value, params->envs,
				*params->last_exit_code);
	if (!value_expanded)
	{
		free(current_value_str);
		return (NULL);
	}
	result = ft_strjoin(current_value_str, value_expanded);
	free(value_expanded);
	free(temp_str);
	return (result);
}

char	*get_token_content_value(t_content_params *params)
{
	t_token_content	*content;
	char			*content_value;

	if (!params || !params->content)
		return (NULL);
	content_value = ft_strdup("");
	if (!content_value)
		return (NULL);
	content = params->content;
	while (content)
	{
		content_value = get_env_value_expanded(content_value, content, params);
		if (!content_value)
			return (NULL);
		content = content->next;
	}
	return (content_value);
}
