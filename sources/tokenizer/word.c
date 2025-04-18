/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/18 15:35:48 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static char	*get_word_in_quotes(char *str, int *index, char delimiter)
{
	char	*word;
	int		word_index;

	(*index)++;
	word_index = 0;
	word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
	while (str[*index] && !is_quote_delimiter(str, index, delimiter))
	{
		if (str[*index] != BACKSLASH_CHR)
			word[word_index++] = str[*index];
		(*index)++;
	}
	word[word_index] = '\0';
	if (str[*index] == delimiter)
		(*index)++;
	return (word);
}

static t_token_content	*append_token_content(
	t_token_content *content, t_token_word_param *param)
{
	t_token_content	*iter;
	char			*temp;
	t_token_content	*new_content;

	if (param->in_quotes)
	{
		temp = param->word;
		param->word = get_word_in_quotes(
				param->str, param->str_index, param->delimiter);
		new_content = new_token_content(ft_strdup(param->word),
				param->delimiter == SINGLE_QUOTE_CHR);
		free(temp);
	}
	else
		new_content = new_token_content(ft_strdup(param->word), FALSE);
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

static t_token_content	*add_only_content_word(
	t_token_content *content, t_token_word_param *param)
{
	if (param->word_index > 0)
	{
		param->word[param->word_index] = '\0';
		content = append_token_content(content, param);
		free(param->word);
	}
	return (content);
}

static t_token_content	*process_word_in_quotes(
	t_token_content *content, t_token_word_param *param
)
{
	int	malloc_word_len;

	malloc_word_len = ft_strlen(param->str + *param->str_index);
	param->delimiter = param->str[*param->str_index];
	content = add_only_content_word(content, param);
	param->word = (char *)malloc(sizeof(char) * (malloc_word_len + 1));
	param->word_index = 0;
	param->in_quotes = TRUE;
	content = append_token_content(content, param);
	param->in_quotes = FALSE;
	return (content);
}

void	add_token_word(char *str, int *index, t_token **tokens)
{
	t_token_content		*content;
	t_token_word_param	param;

	content = NULL;
	param.str = str;
	param.word_index = 0;
	param.in_quotes = FALSE;
	param.str_index = index;
	param.word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
	while (str[*index] && !ft_isspace(str[*index]) && !is_metachar(str[*index]))
	{
		if (str[*index] == DOT_AND_COMA_CHR)
			break ;
		if (str[*index] == SINGLE_QUOTE_CHR || str[*index] == DOUBLE_QUOTE_CHR)
		{
			content = process_word_in_quotes(content, &param);
			continue ;
		}
		if (str[*index] != BACKSLASH_CHR)
			param.word[param.word_index++] = str[*index];
		(*index)++;
	}
	content = add_only_content_word(content, &param);
	add_token(tokens, new_token(content, WORD));
}
