/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/03/22 22:05:24 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_quoted_word(
	char *str, char *word, int *index, char delimiter)
{
	int	word_index;

	word_index = 0;
	(*index)++;
	while (str[*index] && !is_quote_delimiter(str, index, delimiter))
	{
		if (str[*index] != BACKSLASH_CHR)
			word[word_index++] = str[*index];
		(*index)++;
	}
	if (str[*index] == delimiter)
		(*index)++;
	return (word_index);
}

void	add_token_word(char *str, int *index, t_token **tokens)
{
	char	*word;
	int		word_index;
	char	delimiter;

	word_index = 0;
	word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
	while (str[*index] && !ft_isspace(str[*index]) && !is_metachar(str[*index]))
	{
		if (str[*index] == DOT_AND_COMA_CHR)
			break ;
		if (str[*index] == SINGLE_QUOTE_CHR || str[*index] == DOUBLE_QUOTE_CHR)
		{
			delimiter = str[*index];
			word_index += process_quoted_word(
					str, word + word_index, index, delimiter);
			continue ;
		}
		if (str[*index] != BACKSLASH_CHR)
			word[word_index++] = str[*index];
		(*index)++;
	}
	word[word_index] = '\0';
	if (word_index == 0)
		return (free(word));
	add_token(tokens, new_token(word, WORD));
}

static void	add_token_meta(char *input, int *index, t_token **tokens)
{
	if (input[*index] == PIPE_CHR)
		add_token(tokens, new_token(ft_strdup(PIPE_STR), PIPE));
	else if (input[*index] == REDIRECT_IN_CHR)
	{
		if (input[*index + 1] == REDIRECT_IN_CHR)
		{
			add_token(tokens, new_token(ft_strdup(HEREDOC_STR), HEREDOC));
			(*index)++;
		}
		else
			add_token(tokens,
				new_token(ft_strdup(REDIRECT_IN_STR), REDIRECT_IN));
	}
	else if (input[*index] == REDIRECT_OUT_CHR)
	{
		if (input[*index + 1] == REDIRECT_OUT_CHR)
		{
			add_token(tokens, new_token(ft_strdup(APPEND_STR), APPEND));
			(*index)++;
		}
		else
			add_token(tokens,
				new_token(ft_strdup(REDIRECT_OUT_STR), REDIRECT_OUT));
	}
	(*index)++;
}

t_token	*tokenize_input(char *input)
{
	int		index;
	int		is_metachar;
	t_token	*tokens;

	index = 0;
	tokens = NULL;
	while (input[index])
	{
		if (input[index] == DOT_AND_COMA_CHR)
			break ;
		if (ft_isspace(input[index]))
		{
			index++;
			continue ;
		}
		is_metachar = (input[index] == PIPE_CHR
				|| input[index] == REDIRECT_IN_CHR
				|| input[index] == REDIRECT_OUT_CHR);
		if (is_metachar)
			add_token_meta(input, &index, &tokens);
		else
			add_token_word(input, &index, &tokens);
	}
	return (tokens);
}
