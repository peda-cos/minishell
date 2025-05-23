/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/18 15:27:01 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

/**
 * @brief Tokenizes an input string into a linked list of tokens
 * @param input The input string to tokenize
 * @return A linked list of tokens, or NULL if input is NULL
 * @note Processes input character by character:
 *       - Skips spaces and backslash characters
 *       - Identifies and adds redirect tokens (|, <, >)
 *       - Processes word tokens for all other characters
 *       - Stops processing at semicolon
 */
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
		if (ft_isspace(input[index]) || input[index] == BACKSLASH_CHR)
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
