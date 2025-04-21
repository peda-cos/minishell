/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/21 18:02:43 by peda-cos         ###   ########.fr       */
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

char	*get_token_content_value(t_content_params *params)
{
	t_token_content	*content;
	char			*final_value;
	char			*current_segment;
	char			*temp_value;

	if (!params || !params->content)
		return (ft_strdup(""));
	final_value = ft_strdup("");
	if (!final_value)
		return (NULL);
	content = params->content;
	while (content)
	{
		if (content->in_single_quotes)
			current_segment = ft_strdup(content->value);
		else
			current_segment = expand_variable(content->value, params->envs,
					*params->last_exit_code);
		if (!current_segment)
		{
			free(final_value);
			return (NULL);
		}
		temp_value = final_value;
		final_value = ft_strjoin(final_value, current_segment);
		free(temp_value);
		free(current_segment);
		if (!final_value)
			return (NULL);
		content = content->next;
	}
	return (final_value);
}
