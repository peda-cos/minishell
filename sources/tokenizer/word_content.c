/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_content.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/04 19:09:07 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

/**
	* @brief Adds a new token content to the end of the linked list
	* @param content The current token content
	* @param tkz The tokenizer structure containing the new token content
	* @return Pointer to the updated token content
	* @note Allocates memory for the new token content and links it to the list
	*/
t_token_content	*add_only_content_word(t_token_content *content,
	t_tokenizer *tkz)
{
	if (tkz->word_index > 0)
	{
		tkz->word[tkz->word_index] = '\0';
		content = append_token_content(content, tkz);
	}
	return (content);
}

/**
	* @brief Creates a new token content based on the tokenizer state
	* @param tkz The tokenizer structure containing the current state
	* @return A new token content structure, or NULL on failure
	* @note Allocates memory for the new token content and sets its value
	*       based on the current word and quotes status
	*/
t_token_content	*get_new_token_content(t_tokenizer *tkz)
{
	char			*temp;
	t_token_content	*new_content;

	if (tkz->in_quotes)
	{
		temp = tkz->word;
		tkz->word = get_word_in_quotes(tkz->str, tkz->str_index,
				tkz->delimiter);
		if (!tkz->word)
		{
			free(temp);
			return (NULL);
		}
		new_content = new_token_content(ft_strdup(tkz->word), TRUE,
				tkz->delimiter == SINGLE_QUOTE_CHR);
		free(temp);
	}
	else
		new_content = new_token_content(ft_strdup(tkz->word), FALSE, FALSE);
	return (new_content);
}

/**
	* @brief Appends a new token content to the end of the linked list
	* @param content The current token content
	* @param tkz The tokenizer structure containing the new token content
	* @return Pointer to the updated token content
	* @note Allocates memory for the new token content and links it to the list
	*/
t_token_content	*append_token_content(
	t_token_content *content, t_tokenizer *tkz)
{
	t_token_content	*iter;
	t_token_content	*new_content;

	new_content = get_new_token_content(tkz);
	if (!content)
	{
		content = new_content;
		return (content);
	}
	iter = content;
	while (iter->next)
		iter = iter->next;
	iter->next = new_content;
	return (content);
}
