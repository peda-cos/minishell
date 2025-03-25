/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:40:39 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/25 01:51:38 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static int	is_redirection(t_token_type type)
{
	return (type == REDIRECT_IN || type == REDIRECT_OUT || type == APPEND
		|| type == HEREDOC);
}

static void	set_redirection_target(t_command *cmd, t_token *token,
		t_token *target)
{
	if (token->type == REDIRECT_IN)
		cmd->input_file = ft_strdup(target->value);
	else if (token->type == REDIRECT_OUT)
	{
		cmd->output_file = ft_strdup(target->value);
		cmd->append = 0;
	}
	else if (token->type == APPEND)
	{
		cmd->output_file = ft_strdup(target->value);
		cmd->append = 1;
	}
	else if (token->type == HEREDOC)
		cmd->heredoc_delim = ft_strdup(target->value);
}

static int	handle_redirection(t_command *cmd, t_token **token_ptr)
{
	t_token	*token;

	token = *token_ptr;
	*token_ptr = token->next;
	if (!*token_ptr || (*token_ptr)->type != WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n",
			STDERR_FILENO);
		return (0);
	}
	set_redirection_target(cmd, token, *token_ptr);
	*token_ptr = (*token_ptr)->next;
	return (1);
}

static t_token	*process_token(t_token *token, t_parser_context *parser)
{
	if (!parser->current)
	{
		parser->current = append_command(&parser->head);
		if (!parser->current)
			return (NULL);
	}
	if (token->type == PIPE)
	{
		parser->current = NULL;
		return (token->next);
	}
	if (is_redirection(token->type))
	{
		if (!handle_redirection(parser->current, &token))
			return (token);
		return (token);
	}
	if (token->type == WORD)
		return (process_word_tokens(parser->current, token, parser->env,
				parser->last_exit));
	return (token->next);
}

t_command	*parse_tokens(t_token *tokens, char **env, int last_exit)
{
	t_parser_context	parser;

	parser.head = NULL;
	parser.current = NULL;
	parser.env = env;
	parser.last_exit = last_exit;
	while (tokens)
		tokens = process_token(tokens, &parser);
	return (parser.head);
}
