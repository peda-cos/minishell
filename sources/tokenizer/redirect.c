/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/04 19:36:08 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

/**
 * @brief Creates a new token for input redirection
 * @param input The input string
 * @param index The current index in the input string
 * @return A new token for input redirection
 * @note If the next character is also a redirect in character, it creates a
 *       heredoc token instead. Otherwise, it creates a redirect in token.
 */
static t_token	*get_redirect_in_token(char *input, int *index)
{
	t_token_content	*content;

	if (input[*index + 1] == REDIRECT_IN_CHR)
	{
		(*index)++;
		content = new_token_content(ft_strdup(HEREDOC_STR), FALSE, FALSE);
		return (new_token(content, HEREDOC));
	}
	content = new_token_content(ft_strdup(REDIRECT_IN_STR), FALSE, FALSE);
	return (new_token(content, REDIRECT_IN));
}

/**
 * @brief Creates a new token for output redirection
 * @param input The input string
 * @param index The current index in the input string
 * @return A new token for output redirection
 * @note If the next character is also a redirect out character, it creates an
 *       append token instead. Otherwise, it creates a redirect out token.
 */
static t_token	*get_redirect_out_token(char *input, int *index)
{
	t_token_content	*content;

	if (input[*index + 1] == REDIRECT_OUT_CHR)
	{
		(*index)++;
		content = new_token_content(ft_strdup(APPEND_STR), FALSE, FALSE);
		return (new_token(content, APPEND));
	}
	content = new_token_content(ft_strdup(REDIRECT_OUT_STR), FALSE, FALSE);
	return (new_token(content, REDIRECT_OUT));
}

/**
 * @brief Gets the appropriate redirect token based on the input character
 * @param input The input string
 * @param index The current index in the input string
 * @return A new token for redirection
 * @note Delegates to the specific function based on the input character
 */
static t_token	*get_redirect_token(char *input, int *index)
{
	if (input[*index] == REDIRECT_IN_CHR)
		return (get_redirect_in_token(input, index));
	else
		return (get_redirect_out_token(input, index));
}

/**
 * @brief Adds a token for redirection to the token list
 * @param input The input string
 * @param index The current index in the input string
 * @param tokens The token list to add the new token to
 * @return void
 * @note Increments the index after adding the token
 *       Handles pipe tokens and file descriptor detection
 */
void	add_token_redirect(char *input, int *index, t_token **tokens)
{
	char			chr;
	t_token			*token;
	t_token_content	*content;
	t_token			*last_token;

	chr = input[*index];
	if (chr == PIPE_CHR)
	{
		content = new_token_content(ft_strdup(PIPE_STR), FALSE, FALSE);
		token = new_token(content, PIPE);
	}
	else
	{
		if (input && *index > 0 && ft_isdigit(input[*index - 1]))
		{
			last_token = get_last_token(tokens);
			if (last_token->length == 1)
				last_token->type = FILE_DESCRIPTOR;
		}
		token = get_redirect_token(input, index);
	}
	add_token(tokens, token);
	(*index)++;
}
