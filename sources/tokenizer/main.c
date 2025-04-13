/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/12 22:40:35 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static t_token_content	*append_token_content(
	t_token_content *content, t_token_content *new)
{
	t_token_content	*iter;

	if (!content)
	{
		content = new;
		return (content);
	}
	iter = content;
	while (iter->next)
		iter = iter->next;
	iter->next = new;
	return (content);
}

static t_token_content	*new_token_content(char *value, int in_single_quotes)
{
	t_token_content	*content;

	if (!value)
		return (NULL);
	content = malloc(sizeof(t_token_content));
	content->next = NULL;
	content->value = value;
	content->in_single_quotes = in_single_quotes;
	return (content);
}

static char	*get_word_in_quotes(
	char *str, int *index, char delimiter)
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

static void	add_token_word(char *str, int *index, t_token **tokens)
{
	char			*word;
	char			delimiter;
	t_token_content	*content;
	int				word_index;

	word_index = 0;
	content	= NULL;
	word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
	while (str[*index] && !ft_isspace(str[*index]) && !is_metachar(str[*index]))
	{
		if (str[*index] == DOT_AND_COMA_CHR)
			break ;
		if (str[*index] == SINGLE_QUOTE_CHR || str[*index] == DOUBLE_QUOTE_CHR)
		{
			delimiter = str[*index];
			word[word_index] = '\0';
			content = append_token_content(content,
					new_token_content(ft_strdup(word), FALSE));
			free(word);
			word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
			word_index = 0;
			content = append_token_content(content,
					new_token_content(get_word_in_quotes(str, index, delimiter),
						delimiter == SINGLE_QUOTE_CHR));
			continue ;
		}
		if (str[*index] != BACKSLASH_CHR)
			word[word_index++] = str[*index];
		(*index)++;
	}
	word[word_index] = '\0';
	if (word_index > 0)
	{
		content = append_token_content(content,
				new_token_content(ft_strdup(word), FALSE));
		free(word);
	}
	// Precisa validar se contents existe ?
	add_token(tokens, new_token(content, WORD));
}

static t_token	*get_redirect_token(char *input, int *index)
{
	t_token_content	*content;

	if (input[*index] == REDIRECT_IN_CHR)
	{
		if (input[*index + 1] == REDIRECT_IN_CHR)
		{
			(*index)++;
			content = new_token_content(ft_strdup(HEREDOC_STR), FALSE);
			return (new_token(content, HEREDOC));
		}
		else
			return (new_token(new_token_content(
						ft_strdup(REDIRECT_IN_STR), FALSE), REDIRECT_IN));
	}
	if (input[*index + 1] == REDIRECT_OUT_CHR)
	{
		(*index)++;
		content = new_token_content(ft_strdup(APPEND_STR), FALSE);
		return (new_token(content, APPEND));
	}
	else
		return (new_token(new_token_content(
					ft_strdup(REDIRECT_OUT_STR), FALSE), REDIRECT_OUT));
}

static void	add_token_redirect(char *input, int *index, t_token **tokens)
{
	char			chr;
	t_token			*token;
	t_token_content	*content;
	t_token			*last_token;

	chr = input[*index];
	if (chr == PIPE_CHR)
	{
		content = new_token_content(ft_strdup(PIPE_STR), FALSE);
		token = new_token(content, PIPE);
	}
	else
	{
		if (ft_isdigit(input[*index - 1]))
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

t_token	*tokenize_input(char *input)
{
	int		index;
	t_token	*tokens;

	index = 0;
	tokens = NULL;
	if (!input)
		return (NULL);
	while (input[index])
	{
		if (input[index] == DOT_AND_COMA_CHR)
			break ;
		if (ft_isspace(input[index]))
		{
			index++;
			continue ;
		}
		if (input[index] == PIPE_CHR
			|| input[index] == REDIRECT_IN_CHR
			|| input[index] == REDIRECT_OUT_CHR)
			add_token_redirect(input, &index, &tokens);
		else
			add_token_word(input, &index, &tokens);
	}
	return (tokens);
}
