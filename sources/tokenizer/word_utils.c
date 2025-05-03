/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/02 22:28:30 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	set_word_param(
	t_tokenizer *tkz, char *str, int *index)
{
	tkz->str = str;
	tkz->word_index = 0;
	tkz->in_quotes = FALSE;
	tkz->str_index = index;
	tkz->word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
	if (!tkz->word)
		return (0);
	return (1);
}

char	*get_word_in_brackets(char *str, int *index)
{
	char	*word;
	int		word_index;

	word_index = 0;
	word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
	if (!word)
		return (NULL);
	while (str[*index] && str[*index] != BRACKET_CLOSE_CHR)
	{
		if (str[*index] != BACKSLASH_CHR)
			word[word_index++] = str[*index];
		(*index)++;
	}
	if (str[*index] != BRACKET_CLOSE_CHR)
	{
		ft_putstr_fd("minishell: syntax error: unclosed brackets\n",
			STDERR_FILENO);
		free(word);
		return (NULL);
	}
	word[word_index++] = str[*index];
	(*index)++;
	word[word_index] = '\0';
	return (word);
}

char	*get_word_in_quotes(char *str, int *index, char delimiter)
{
	char	*word;
	int		word_index;

	(*index)++;
	word_index = 0;
	word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
	if (!word)
		return (NULL);
	while (str[*index] && !is_quote_delimiter(str, index, delimiter))
	{
		if (str[*index] != BACKSLASH_CHR)
			word[word_index++] = str[*index];
		(*index)++;
	}
	word[word_index] = '\0';
	if (str[*index] != delimiter)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n",
			STDERR_FILENO);
		free(word);
		return (NULL);
	}
	(*index)++;
	return (word);
}

t_token_content	*process_word_in_brackets(
	t_token_content *content, t_tokenizer *tkz)
{
	int		malloc_word_len;

	malloc_word_len = ft_strlen(tkz->str + *tkz->str_index);
	content = add_only_content_word(content, tkz);
	if (!content && tkz->word_index > 0)
		return (NULL);
	free(tkz->word);
	tkz->word = (char *)malloc(sizeof(char) * (malloc_word_len + 1));
	if (!tkz->word)
		return (NULL);
	tkz->word_index = 0;
	tkz->word = get_word_in_brackets(tkz->str, tkz->str_index);
	if (!tkz->word)
		return (NULL);
	tkz->in_quotes = FALSE;
	content = append_token_content(content, tkz);
	if (!content)
		return (NULL);
	return (content);
}

t_token_content	*process_word_in_quotes(t_token_content *content,
		t_tokenizer *tkz)
{
	int	malloc_word_len;

	malloc_word_len = ft_strlen(tkz->str + *tkz->str_index);
	tkz->delimiter = tkz->str[*tkz->str_index];
	content = add_only_content_word(content, tkz);
	if (!content && tkz->word_index > 0)
		return (NULL);
	free(tkz->word);
	tkz->word = (char *)malloc(sizeof(char) * (malloc_word_len + 1));
	if (!tkz->word)
		return (NULL);
	tkz->word_index = 0;
	tkz->in_quotes = TRUE;
	content = append_token_content(content, tkz);
	if (!content)
		return (NULL);
	tkz->in_quotes = FALSE;
	return (content);
}
