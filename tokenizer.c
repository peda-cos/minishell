/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 00:49:55 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/15 21:28:21 by jlacerda         ###   ########.fr       */
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

static int	ft_is_metachar(char c)
{
	return (c == PIPE_CHR || c == REDIRECT_IN_CHR || c == REDIRECT_OUT_CHR);
}

static int ft_is_quote_delimiter(char *str, int *index, char delimiter)
{
	char	chr;
	int		is_quote;

	chr = str[*index];
	is_quote = (chr == delimiter);
	if (*index == 0 && is_quote)
		return (TRUE);
	else if (is_quote && str[*index - 1] != BACKSLASH_CHR)
		return (TRUE);
	return (FALSE);
}

static void add_word_in_quotes(t_token **tokens, char *str, int *index)
{
	char	*word;
	char	delimiter;
	int		i;

	i = 0;
	delimiter = str[*index];
	(*index)++;
	word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
	while (str[*index] && (!ft_is_quote_delimiter(str, index, delimiter)
		|| (!ft_isspace(str[*index + 1]) && !ft_is_metachar(str[*index + 1]))))
	{
		if (str[*index] != delimiter)
		{
			word[i] = str[*index];
			i++;
		}
		(*index)++;
	}
	word[i] = '\0';
	if (i == 0)
		return (free(word));
	if (str[*index] == delimiter)
		(*index)++;
	add_token(tokens, new_token(word, WORD));
}


static void add_word_without_quotes(t_token **tokens, char *str, int *index)
{
	char	*word;
	int		i;

	i = 0;
	word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
	while (str[*index] && !ft_isspace(str[*index])
		&& !ft_is_metachar(str[*index]))
	{
		if (str[*index] == BACKSLASH_CHR && str[*index + 1] == BACKSLASH_CHR)
		{
			word[i] = str[*index + 1];
			i++;
			(*index) += 2;
			continue ;
		}
		word[i] = str[*index];
		i++;
		(*index)++;
	}
	if (i == 0)
		return (free(word));
	word[i] = '\0';
	add_token(tokens, new_token(word, WORD));
}

static void	add_word_with_quotes(t_token **tokens, char *str, int *index)
{
	char	*word;
	int		word_index;
	char	delimiter;

	delimiter = 0;
	word_index = 0;
	word = (char *)malloc(sizeof(char) * (ft_strlen(str + *index) + 1));
	while (str[*index]
		&& !ft_isspace(str[*index]) && !ft_is_metachar(str[*index]))
	{
		if (str[*index] == SINGLE_QUOTE_CHR || str[*index] == DOUBLE_QUOTE_CHR)
		{
			delimiter = str[*index];
			(*index)++;
			while (str[*index] && !ft_is_quote_delimiter(str, index, delimiter))
			{
				if (str[*index] != BACKSLASH_CHR)
				{
					word[word_index] = str[*index];
					word_index++;
				}
				(*index)++;
			}
			if (str[*index] == delimiter)
				(*index)++;
			continue ;
		}
		word[word_index] = str[*index];
		word_index++;
		(*index)++;
	}
	word[word_index] = '\0';
	if (word_index == 0)
	{
		free(word);
		return ;
	}
	add_token(tokens, new_token(word, WORD));
}
static void	add_token_word(t_token **tokens, char *str, int *index)
{
	int	has_quote;
	int	i;

	i = *index;
	has_quote = FALSE;
	if (str[i] == SINGLE_QUOTE_CHR || str[i] == DOUBLE_QUOTE_CHR)
		add_word_in_quotes(tokens, str, index);
	else
	{
		i = *index;
		while (str[i] && !ft_isspace(str[i]) && !has_quote)
		{
			has_quote = str[i] == SINGLE_QUOTE_CHR
				|| str[i] == DOUBLE_QUOTE_CHR;
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
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
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
