/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_content.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/02 22:28:30 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

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
