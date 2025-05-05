/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/04 19:08:56 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

/**
 * @brief Checks if the character is a metacharacter
 * @param c The character to check
 * @return 1 if it is a metacharacter, 0 otherwise
 * @note Metacharacters include pipe, input redirect, and output redirect
 */
int	is_metachar(char c)
{
	return (c == PIPE_CHR
		|| c == REDIRECT_IN_CHR
		|| c == REDIRECT_OUT_CHR);
}

/**
 * @brief Checks if the character is a quote delimiter
 * @param str The string containing the character
 * @param index Pointer to the current position in the string
 * @param delimiter The quote delimiter to check for
 * @return 1 if it is a quote delimiter, 0 otherwise
 * @note Handles escaped quotes and checks for valid quote positions
 */
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

/**
 * @brief Creates a new token with the given content and type
 * @param content The content of the token
 * @param type The type of the token
 * @return A pointer to the newly created token, or NULL on failure
 * @note Allocates memory for the token and calculates its length
 */
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

/**
 * @brief Adds a new token to the end of the token list
 * @param tokens A pointer to the head of the token list
 * @param new The new token to add
 * @return void
 * @note Handles empty lists and
	* updates prev/next pointers appropriately
 */
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

/**
 * @brief Gets the last token in the token list
 * @param tokens A pointer to the head of the token list
 * @return A pointer to the last token, or NULL if the list is empty
 * @note Iterates through the list to find the last token
 */
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
