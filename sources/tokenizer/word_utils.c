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

/**
	* @brief Sets the parameters for the tokenizer structure
	* @param tkz The tokenizer structure to set
	* @param str The input string to tokenize
	* @param index The current index in the input string
	* @return 1 on success, 0 on failure
	* @note Allocates memory for the word and initializes the tokenizer structure
	*/
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

/**
	* @brief Gets the word inside brackets from the input string
	* @param str The input string to tokenize
	* @param index The current index in the input string
	* @return A pointer to the word inside brackets, or NULL on failure
	* @note Allocates memory for the word and checks for unclosed brackets
	*/
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

/**
	* @brief Gets the word inside quotes from the input string
	* @param str The input string to tokenize
	* @param index The current index in the input string
	* @param delimiter The quote delimiter (single or double quote)
	* @return A pointer to the word inside quotes, or NULL on failure
	* @note Allocates memory for the word and checks for unclosed quotes
	*/
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

/**
	* @brief Processes a word inside brackets and appends it to the token content
	* @param content The current token content
	* @param tkz The tokenizer structure containing the word and index
	* @return A pointer to the updated token content, or NULL on failure
	* @note Allocates memory for the new word and appends it to the content list
	*/
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

/**
	* @brief Processes a word inside quotes and appends it to the token content
	* @param content The current token content
	* @param tkz The tokenizer structure containing the word and index
	* @return A pointer to the updated token content, or NULL on failure
	* @note Allocates memory for the new word and appends it to the content list
	*/
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
