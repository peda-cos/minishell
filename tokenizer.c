/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 00:49:55 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/20 00:45:23 by jlacerda         ###   ########.fr       */
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
	if (input[*index] == PIPE_CHR){
		add_token(tokens, new_token(ft_strdup(PIPE_STR), PIPE));
	}
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

// static char	*formats_token_word(char *str)
// {
// 	int		i;
// 	int		len;
// 	char	*formatted_str;

// 	len = ft_strlen(str);
// 	formatted_str = (char *)malloc(sizeof(char) + (len + 1));
// 	if (!formatted_str)
// 		return (NULL);
// 	i = 0;
// 	if (str[0] == SINGLE_QUOTE_CHR || str[0] == DOUBLE_QUOTE_CHR)
// 	{
// 		i++;
// 		while (str[i] && str[i] != str[0])
// 		{
// 			formatted_str[i] = str[i];
// 			i++;
// 		}
// 		formatted_str[i] = '\0';
// 		return (formatted_str);
// 	}
// 	return (str);
// }


char *ft_strtok2(char *input, const char *delimiter)
{
	static char	*backup;
	char		*token;

	if (input != NULL)
		backup = input;
	if (backup == NULL)
		return (NULL);
	// Ignorar delimitadores iniciais
	while (*backup && strchr(delimiter, *backup))
		backup++;
	if (*backup == '\0')
		return (NULL);
	token = backup;
	// Encontrar o próximo delimitador
	while (*backup)
	{
		if (strchr(delimiter, *backup))
		{
			*backup = '\0';
			backup++;
			break ;
		}
		backup++;
	}
	// Se chegamos ao final da string, definir backup como NULL
	if (*backup == '\0')
		backup = NULL;
	return (token);
}

#include <stdio.h>
#include <string.h>
t_token	*tokenize_input(char *input)
{
	t_token	*tokens_redirect;
	char	*token;
	int		i;

	tokens_redirect = NULL;
	token = NULL;
	i = 0;
	int input_len = ft_strlen(input);
	char *input_dup	= ft_strdup(input);
	while (input[i] && i < input_len)
	{
		if (input[i] && ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		if (input[i] == PIPE_CHR
			|| input[i] == REDIRECT_IN_CHR
			|| input[i] == REDIRECT_OUT_CHR)
			add_token_type(input, &i, &tokens_redirect);
		else if (input[i] == SINGLE_QUOTE_CHR || input[i] == DOUBLE_QUOTE_CHR)
		{ // echo "|  |"	|> file.txt
			char *quote = NULL;
			if (input[i] == SINGLE_QUOTE_CHR)
				quote = SINGLE_QUOTE_STR;
			else
				quote = DOUBLE_QUOTE_STR;
			if (token)
				token = strtok(NULL, quote);
			else
				token = strtok(input_dup, quote);
			add_token(&tokens_redirect, new_token(token, WORD));
			i += ft_strlen(token) + 2;
		}
		else {
			if (token)
				token = strtok(NULL, " \t\n|><\"'");
			else
				token = strtok(input_dup, " \t\n|><\"'");
			add_token(&tokens_redirect, new_token(token, WORD));
			i += ft_strlen(token);
			continue;
		}
		i++;
	}

	// t_token	*tokens_words;

	// tokens_words = NULL;
	// char *delimiter = " \t\n|><";
	// token = ft_strtok(input, delimiter);
	// printf("token-> %s\n", token);
	// while (token)
	// {
	// 	token	= ft_strtok(NULL, delimiter);
	// 	if (token == NULL)
	// 		break ;
	// 	t_token	*new_token = malloc(sizeof(t_token));
	// 	new_token->value = token;
	// 	new_token->type = WORD;
	// 	new_token->next = NULL;
	// 	add_token(&tokens_words, new_token);
	// }

	// [1]->[2]
	// [a]->[b]

	// [1]->[a]->[2]->[b]

	// t_token *joined_tokens = NULL;
	// t_token	*next_token_words;
	// t_token	*next_token_redirect;
	// t_token	*tmp;
	// while (tokens_words		!= NULL)
	// {
	// 	printf("word-> %s\n", tokens_words->value);
	// 	tokens_words = tokens_words->next;
	// }
	while (tokens_redirect	!= NULL)
	{
		printf("token-> %s\n", tokens_redirect->value);
		tokens_redirect = tokens_redirect->next;
	}
	
	// while (tokens_words  != NULL)
	// {
	// 	next_token_words = tokens_words->next;
	// 	next_token_redirect = tokens_redirect->next;

	// 	tokens_words->next = tokens_redirect;
	// 	tokens_redirect->next = next_token_words;


	// }

	return (NULL);
}
