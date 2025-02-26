/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 00:49:55 by peda-cos          #+#    #+#             */
/*   Updated: 2025/02/26 16:41:57 by peda-cos         ###   ########.fr       */
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

t_token	*tokenize_input(char *input)
{
	t_token	*tokens;
	int		i;
	int		start;
	char	*word;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && (input[i] == ' ' || input[i] == '\t'))
			i++;
		if (!input[i])
			break ;
		if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			if (input[i] == '|')
				add_token(&tokens, new_token(ft_strdup("|"), PIPE));
			else if (input[i] == '<')
			{
				if (input[i + 1] == '<')
				{
					add_token(&tokens, new_token(ft_strdup("<<"), HEREDOC));
					i++;
				}
				else
					add_token(&tokens, new_token(ft_strdup("<"), REDIRECT_IN));
			}
			else if (input[i] == '>')
			{
				if (input[i + 1] == '>')
				{
					add_token(&tokens, new_token(ft_strdup(">>"), APPEND));
					i++;
				}
				else
					add_token(&tokens, new_token(ft_strdup(">"), REDIRECT_OUT));
			}
			i++;
		}
		else if (input[i] == '\'')
		{
			i++;
			start = i;
			while (input[i] && input[i] != '\'')
				i++;
			word = ft_substr(input, start, i - start);
			add_token(&tokens, new_token(word, WORD));
			if (input[i] == '\'')
				i++;
		}
		else if (input[i] == '\"')
		{
			i++;
			start = i;
			while (input[i] && input[i] != '\"')
				i++;
			word = ft_substr(input, start, i - start);
			add_token(&tokens, new_token(word, WORD));
			if (input[i] == '\"')
				i++;
		}
		else
		{
			start = i;
			while (input[i] && input[i] != ' ' && input[i] != '\t'
				&& input[i] != '|' && input[i] != '<' && input[i] != '>')
				i++;
			word = ft_substr(input, start, i - start);
			add_token(&tokens, new_token(word, WORD));
		}
	}
	return (tokens);
}
