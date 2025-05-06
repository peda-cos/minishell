/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/05 22:36:28 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

/**
 * @brief Creates a new token content structure
 * @param value The string value to store in the token content
 * @param in_quotes Flag indicating if the content is in double quotes
 * @param in_single_quotes Flag indicating if the content is in single quotes
 * @return Pointer to the newly created token content, or NULL on failure
 * @note Allocates memory for the token content structure
 */
t_token_content	*new_token_content(char *value,
	int in_quotes, int in_single_quotes)
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
	content->in_quotes = in_quotes;
	content->in_single_quotes = in_single_quotes;
	return (content);
}

/**
 * @brief Gets the expanded segment of a token content
 * @param content The token content to expand
 * @param params The parameters for processing the content
 * @param has_error Pointer to the error flag
 * @return The expanded segment, or NULL on error
 * @note Handles variable expansion based on context
 */
static char	*get_segment_expanded(t_token_content *content,
	t_content_params *params, int *has_error)
{
	t_expansion_ctx	expansion_context;

	expansion_context.content = content;
	expansion_context.envs = params->envs;
	expansion_context.has_error_flag_control = has_error;
	expansion_context.last_exit = params->last_exit_code;
	expansion_context.was_expanded = &params->was_expanded;
	return (expand_variable(content->value, &expansion_context));
}

/**
 * @brief Appends the expanded value of the token content to the final value
 * @param final_value The final value string to append to
 * @param content The token content to process
 * @param params The parameters for processing the content
 * @return Pointer to the updated final value string, or NULL on error
 * @note Allocates memory for the new string and frees the old one
 */
static char	*append_to_final_value(char *final_value,
	t_token_content *content, t_content_params *params)
{
	int				has_error;
	char			*temp_value;
	char			*current_segment;

	has_error = FALSE;
	if (content->in_single_quotes)
		current_segment = ft_strdup(content->value);
	else
		current_segment = get_segment_expanded(content, params, &has_error);
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

/**
 * @brief Processes the content of the token and returns the final value
 * @param params The parameters for processing the content
 * @return Pointer to the final value string, or NULL on error
 * @note Allocates memory for the final value string
 */
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
		if (ft_strcmp(content->value, "$") == 0
			&& !content->in_quotes
			&& (content->next && content->next->in_quotes))
		{
			content = content->next;
			continue ;
		}
		final_value = append_to_final_value(final_value, content, params);
		if (!final_value)
			return (NULL);
		content = content->next;
	}
	return (final_value);
}
