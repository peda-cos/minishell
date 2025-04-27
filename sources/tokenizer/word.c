/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/26 22:40:58 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static char	*get_word_in_brackets(char *str, int *index)
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

static char	*get_word_in_quotes(char *str, int *index, char delimiter)
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

static t_token_content	*append_token_content(t_token_content *content,
	t_token_word_param *param)
{
	t_token_content	*iter;
	char			*temp;
	t_token_content	*new_content;

	if (param->in_quotes)
	{
		temp = param->word;
		param->word = get_word_in_quotes(param->str, param->str_index,
				param->delimiter);
		if (!param->word)
		{
			free(temp);
			return (NULL);
		}
		new_content = new_token_content(ft_strdup(param->word), TRUE,
				param->delimiter == SINGLE_QUOTE_CHR);
		free(temp);
	}
	else
		new_content = new_token_content(ft_strdup(param->word), FALSE, FALSE);
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

static t_token_content	*add_only_content_word(t_token_content *content,
		t_token_word_param *param)
{
	if (param->word_index > 0)
	{
		param->word[param->word_index] = '\0';
		content = append_token_content(content, param);
	}
	return (content);
}

static t_token_content	*process_word_in_brackets(
	t_token_content *content, t_token_word_param *param)
{
	int		malloc_word_len;

	malloc_word_len = ft_strlen(param->str + *param->str_index);
	content = add_only_content_word(content, param);
	if (!content && param->word_index > 0)
		return (NULL);
	free(param->word);
	param->word = (char *)malloc(sizeof(char) * (malloc_word_len + 1));
	if (!param->word)
		return (NULL);
	param->word_index = 0;
	param->word = get_word_in_brackets(param->str, param->str_index);
	if (!param->word)
		return (NULL);
	param->in_quotes = FALSE;
	content = append_token_content(content, param);
	if (!content)
		return (NULL);
	return (content);
}

static t_token_content	*process_word_in_quotes(t_token_content *content,
		t_token_word_param *param)
{
	int	malloc_word_len;

	malloc_word_len = ft_strlen(param->str + *param->str_index);
	param->delimiter = param->str[*param->str_index];
	content = add_only_content_word(content, param);
	if (!content && param->word_index > 0)
		return (NULL);
	free(param->word);
	param->word = (char *)malloc(sizeof(char) * (malloc_word_len + 1));
	if (!param->word)
		return (NULL);
	param->word_index = 0;
	param->in_quotes = TRUE;
	content = append_token_content(content, param);
	if (!content)
		return (NULL);
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
	if (!param.word)
		return ;
	while (str[*index] && !ft_isspace(str[*index]) && !is_metachar(str[*index]))
	{
		if (str[*index] == DOT_AND_COMA_CHR)
			break ;
		if (str[*index] == DOLLAR_CHR
			&& str[*index + 1] == BRACKET_OPEN_CHR)
		{
			content = process_word_in_brackets(content, &param);
			if (!content && param.word_index > 0)
			{
				free(param.word);
				return ;
			}
			continue ;
		}
		if (str[*index] == SINGLE_QUOTE_CHR || str[*index] == DOUBLE_QUOTE_CHR)
		{
			content = process_word_in_quotes(content, &param);
			if (!content && param.word_index > 0)
			{
				free(param.word);
				return ;
			}
			continue ;
		}
		if (str[*index] != BACKSLASH_CHR)
			param.word[param.word_index++] = str[*index];
		(*index)++;
	}
	content = add_only_content_word(content, &param);
	free(param.word);
	if (content)
		add_token(tokens, new_token(content, WORD));
}
