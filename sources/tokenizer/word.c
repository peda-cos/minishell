/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/02 22:31:18 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static int	process_with_brackets(char *str,
	int *index, t_token_content **content, t_tokenizer *tkz)
{
	if (str[*index] == DOLLAR_CHR
		&& str[*index + 1] == BRACKET_OPEN_CHR)
	{
		*content = process_word_in_brackets(*content, tkz);
		if (!*content && tkz->word_index > 0)
		{
			free(tkz->word);
			return (-1);
		}
		return (1);
	}
	return (0);
}

static int	process_with_quotes(char *str,
	int *index, t_token_content **content, t_tokenizer *tkz)
{
	if (str[*index] == SINGLE_QUOTE_CHR || str[*index] == DOUBLE_QUOTE_CHR)
	{
		*content = process_word_in_quotes(*content, tkz);
		if (!*content && tkz->word_index > 0)
		{
			free(tkz->word);
			return (-1);
		}
		return (1);
	}
	return (0);
}

static int	process_word_chars(char *str,
	int *index, t_token_content **content, t_tokenizer *tkz)
{
	int	process_flag;

	while (str[*index] && !ft_isspace(str[*index]) && !is_metachar(str[*index]))
	{
		if (str[*index] == DOT_AND_COMA_CHR)
			break ;
		process_flag = process_with_brackets(str, index, content, tkz);
		if (process_flag == 1)
			continue ;
		else if (process_flag == -1)
			return (0);
		process_flag = process_with_quotes(str, index, content, tkz);
		if (process_flag == 1)
			continue ;
		else if (process_flag == -1)
			return (0);
		if (str[*index] != BACKSLASH_CHR)
			tkz->word[tkz->word_index++] = str[*index];
		(*index)++;
	}
	return (1);
}

void	add_token_word(char *str, int *index, t_token **tokens)
{
	t_tokenizer		tkz;
	t_token_content	*content;

	content = NULL;
	if (!set_word_param(&tkz, str, index))
		return ;
	if (!process_word_chars(str, index, &content, &tkz))
	{
		free(tkz.word);
		return ;
	}
	content = add_only_content_word(content, &tkz);
	free(tkz.word);
	if (content)
		add_token(tokens, new_token(content, WORD));
}
