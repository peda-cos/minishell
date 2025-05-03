/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:40:39 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/02 20:40:29 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_token	*process_token_word(t_token *token,
	t_parser_context *parser, char **envs, int *last_exit)
{
	t_content_params	content_params;
	char				*arg_val;

	content_params.envs = envs;
	content_params.was_expanded = FALSE;
	content_params.content = token->content;
	content_params.last_exit_code = last_exit;
	arg_val = get_token_content_value(&content_params);
	if (!arg_val)
	{
		cleanup_parser_on_error(parser);
		return (NULL);
	}
	if (!append_argument(parser->current, arg_val))
	{
		cleanup_parser_on_error(parser);
		return (NULL);
	}
	if (content_params.was_expanded)
		parser->was_expanded = TRUE;
	return (token->next);
}

static t_token	*process_token(t_token *token,
	t_parser_context *parser, char **envs, int *last_exit)
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
		if (!handle_redirection(parser->current, &token, envs, last_exit))
		{
			cleanup_parser_on_error(parser);
			return (NULL);
		}
		return (token);
	}
	if (token->type == WORD)
		return (process_token_word(token, parser, envs, last_exit));
	return (token->next);
}

t_command	*parse_tokens(t_token *tokens, char **env, int last_exit)
{
	t_parser_context	parser;
	t_token				*next_token;
	t_token				*tokens_head;

	tokens_head = tokens;
	parser.env = env;
	parser.head = NULL;
	parser.current = NULL;
	parser.was_expanded = FALSE;
	parser.last_exit = last_exit;
	while (tokens)
	{
		next_token = process_token(tokens, &parser, env, &last_exit);
		if (!next_token && !parser.head)
			return (NULL);
		tokens = next_token;
	}
	if (parser.was_expanded)
		update_cmd_args_when_expanded(parser.current, tokens_head);
	return (parser.head);
}
