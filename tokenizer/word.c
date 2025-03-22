/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/03/22 15:47:40 by jlacerda         ###   ########.fr       */
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

void	add_word_with_quotes(t_token **tokens, char *str, int *index)
{
	char	*word;
	int		word_index;
	char	delimiter;

	word_index = 0;
	word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
	while (str[*index] && !ft_isspace(str[*index]) && !is_metachar(str[*index]))
	{
		if (str[*index] == SINGLE_QUOTE_CHR || str[*index] == DOUBLE_QUOTE_CHR)
		{
			delimiter = str[*index];
			word_index += process_quoted_word(
					str, word + word_index, index, delimiter);
			continue ;
		}
		word[word_index++] = str[*index];
		(*index)++;
	}
	word[word_index] = '\0';
	if (word_index == 0)
		return (free(word));
	add_token(tokens, new_token(word, WORD));
}

void	add_word_in_quotes(t_token **tokens, char *str, int *index)
{
	int		i;
	char	*word;
	char	next_char;
	char	quote_delimiter;

	i = 0;
	quote_delimiter = str[*index];
	(*index)++;
	word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
	while (str[*index] && (
			!is_quote_delimiter(str, index, quote_delimiter)
			|| (!ft_isspace(next_char) || !is_metachar(next_char))))
	{
		if (str[*index] != quote_delimiter)
			word[i++] = str[*index];
		(*index)++;
		next_char = str[*index + 1];
	}
	word[i] = '\0';
	if (i == 0)
		return (free(word));
	if (str[*index] == quote_delimiter)
		(*index)++;
	add_token(tokens, new_token(word, WORD));
}

void	add_word_without_quotes(t_token **tokens, char *str, int *index)
{
	int		i;
	char	*word;

	i = 0;
	word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
	while (str[*index] && !ft_isspace(str[*index]) && !is_metachar(str[*index]))
	{
		if (str[*index] == BACKSLASH_CHR && str[*index + 1] == BACKSLASH_CHR)
		{
			word[i] = str[*index + 1];
			(*index) += 2;
			i++;
			continue ;
		}
		word[i] = str[*index];
		(*index)++;
		i++;
	}
	if (i == 0)
		return (free(word));
	word[i] = '\0';
	add_token(tokens, new_token(word, WORD));
}
