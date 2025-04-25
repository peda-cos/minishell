/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/25 00:11:18 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

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

static char	*append_to_final_value(char *final_value,
	t_token_content *content, t_content_params *params)
{
	int		has_error;
	char	*temp_value;
	char	*current_segment;

	has_error = FALSE;
	if (content->in_single_quotes)
		current_segment = ft_strdup(content->value);
	else
		current_segment = expand_variable(content->value,
				params->envs, *params->last_exit_code, &has_error);
	if (has_error)
	{
		free(current_segment);
		free(final_value);
		return (NULL);
	}
	if (!current_segment)
		return (free(final_value), NULL);
	temp_value = final_value;
	final_value = ft_strjoin(final_value, current_segment);
	free(temp_value);
	free(current_segment);
	return (final_value);
}

char	*get_token_content_value(t_content_params *params)
{
	t_token_content	*content;
	char			*final_value;

	if (!params || !params->content)
		return (ft_strdup(""));
	final_value = ft_strdup("");
	if (!final_value)
		return (NULL);
	content = params->content;
	while (content)
	{
		final_value = append_to_final_value(final_value, content, params);
		if (!final_value)
			return (NULL);
		content = content->next;
	}
	return (final_value);
}
