/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 00:49:55 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/10 23:09:12 by jlacerda         ###   ########.fr       */
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

// static void	add_token_type(char *input, int *index, t_token **tokens)
// {
// 	if (input[*index] == PIPE_CHR)
// 		add_token(tokens, new_token(ft_strdup(PIPE_STR), PIPE));
// 	else if (input[*index] == REDIRECT_IN_CHR)
// 	{
// 		if (input[*index + 1] == REDIRECT_IN_CHR)
// 		{
// 			add_token(tokens, new_token(ft_strdup(HEREDOC_STR), HEREDOC));
// 			(*index)++;
// 		}
// 		else
// 			add_token(tokens,
// 				new_token(ft_strdup(REDIRECT_IN_STR), REDIRECT_IN));
// 	}
// 	else if (input[*index] == REDIRECT_OUT_CHR)
// 	{
// 		if (input[*index + 1] == REDIRECT_OUT_CHR)
// 		{
// 			add_token(tokens, new_token(ft_strdup(APPEND_STR), APPEND));
// 			(*index)++;
// 		}
// 		else
// 			add_token(tokens,
// 				new_token(ft_strdup(REDIRECT_OUT_STR), REDIRECT_OUT));
// 	}
// 	(*index)++;
// }

static char	*formats_token_word(char *str)
{
	int		i;
	int		len;
	char	*formatted_str;

	len = ft_strlen(str);
	formatted_str = (char *)malloc(sizeof(char) + (len + 1));
	if (!formatted_str)
		return (NULL);
	i = 0;
	if (str[0] == SINGLE_QUOTE_CHR || str[0] == DOUBLE_QUOTE_CHR)
	{
		i++;
		while (str[i] && str[i] != str[0])
		{
			formatted_str[i] = str[i];
			i++;
		}
		formatted_str[i] = '\0';
		return (formatted_str);
	}
	return (str);
}
#include <stdio.h>
t_token	*tokenize_input(char *input)
{
	t_token	*tokens;
	char	*token;
	int		i;

	// tokens = NULL;
	// i = 0;
	// while (input[i])
	// {
	// 	if (input[i] && ft_isspace(input[i]))
	// 	{
	// 		i++;
	// 		continue ;
	// 	}
	// 	if (input[i] == PIPE_CHR
	// 		|| input[i] == REDIRECT_IN_CHR
	// 		|| input[i] == REDIRECT_OUT_CHR)
	// 		add_token_type(input, &i, &tokens);
	// 	else
	// 		add_token_word(&tokens, input, &i);
	// }

	i = 0;
	tokens = NULL;
	token = ft_strtok(input, " \t\n");
	printf("Token STR: %s\n", token);

	while (token != NULL)
	{
		if (ft_strcmp(token, PIPE_STR) == 0)
			add_token(&tokens, new_token(ft_strdup(PIPE_STR), PIPE));
		else if (ft_strcmp(token, REDIRECT_IN_STR) == 0)
			add_token(&tokens,
				new_token(ft_strdup(REDIRECT_IN_STR), REDIRECT_IN));
		else if (ft_strcmp(token, REDIRECT_OUT_STR) == 0)
			add_token(&tokens,
				new_token(ft_strdup(REDIRECT_OUT_STR), REDIRECT_OUT));
		else if (ft_strcmp(token, APPEND_STR) == 0)
			add_token(&tokens, new_token(ft_strdup(APPEND_STR), APPEND));
		else if (ft_strcmp(token, HEREDOC_STR) == 0)
			add_token(&tokens, new_token(ft_strdup(HEREDOC_STR), HEREDOC));
		else
			add_token(&tokens, new_token(formats_token_word(token), WORD));
		token = ft_strtok(NULL, " \t\n");
	}
	while (tokens)
	{
		printf("Token: %s\n", tokens->value);
		tokens = tokens->next;
	}
	return (tokens);
}
