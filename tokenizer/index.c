/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:18:28 by jlacerda          #+#    #+#             */
/*   Updated: 2025/03/22 15:49:30 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static void	add_token_word(char *str, int *index, t_token **tokens)
{
	int	i;
	int	has_quote;
	int	is_single_quote;
	int	is_double_quote;

	i = *index;
	has_quote = FALSE;
	if (str[i] == SINGLE_QUOTE_CHR || str[i] == DOUBLE_QUOTE_CHR)
		add_word_in_quotes(tokens, str, index);
	else
	{
		i = *index;
		while (str[i] && !ft_isspace(str[i]) && !has_quote)
		{
			is_single_quote = str[i] == SINGLE_QUOTE_CHR;
			is_double_quote = str[i] == DOUBLE_QUOTE_CHR;
			has_quote = (is_single_quote || is_double_quote);
			i++;
		}
		if (has_quote)
			add_word_with_quotes(tokens, str, index);
		else
			add_word_without_quotes(tokens, str, index);
	}
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
