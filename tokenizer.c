/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 00:49:55 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/08 19:29:17 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

static void	add_token(t_token **tokens, t_token *new)
{
	t_token	*tmp;

	if (!*tokens)
	{
		*tokens = new;
		return ;
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static void	add_token_type(char *input, int *index, t_token **tokens)
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

static char	*add_token_word(
	t_token **tokens, char *str, int *index)
{
	int		start;
	char	*word;
	char	*skippers;

	if (str[*index] == SINGLE_QUOTE_CHR)
		skippers = SINGLE_QUOTE_STR;
	else if (str[*index] == DOUBLE_QUOTE_CHR)
		skippers = DOUBLE_QUOTE_STR;
	else
		skippers = SPACES_AND_TOKENS_TYPE;
	if (str[*index] == SINGLE_QUOTE_CHR || str[*index] == DOUBLE_QUOTE_CHR)
		(*index)++;
	start = *index;
	while (str[*index] && !ft_strchr(skippers, str[*index]))
		(*index)++;
	word = ft_substr(str, start, *index - start);
	add_token(tokens, new_token(word, WORD));
	if (str[*index] == SINGLE_QUOTE_CHR || str[*index] == DOUBLE_QUOTE_CHR)
		(*index)++;
	return (word);
}

t_token	*tokenize_input(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] && ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		if (input[i] == PIPE_CHR
			|| input[i] == REDIRECT_IN_CHR
			|| input[i] == REDIRECT_OUT_CHR)
			add_token_type(input, &i, &tokens);
		else
			add_token_word(&tokens, input, &i);
	}
	return (tokens);
}
