/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:40:39 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/12 21:43:53 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static int	is_redirection(t_token_type type)
{
	return (type == REDIRECT_IN || type == REDIRECT_OUT || type == APPEND
		|| type == HEREDOC);
}

static void	set_redirection_target(t_command *cmd, t_token *token,
		t_content_params *content_params)
{
	if (token->type == REDIRECT_IN)
		cmd->input_file = get_content_value(content_params);
	else if (token->type == REDIRECT_OUT)
	{
		cmd->output_file = get_content_value(content_params);
		cmd->append = 0;
	}
	else if (token->type == APPEND)
	{
		cmd->output_file = get_content_value(content_params);
		cmd->append = 1;
	}
	else if (token->type == HEREDOC)
		cmd->heredoc_delim = get_content_value(content_params);
}

static int	handle_redirection(t_command *cmd,
	t_token **token_ptr, char **envs, int *last_exit)
{
	t_token				*token;
	t_content_params	params;

	token = *token_ptr;
	*token_ptr = token->next;
	if (!*token_ptr || (*token_ptr)->type != WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n",
			STDERR_FILENO);
		return (0);
	}
	params.envs = envs;
	params.last_exit_code = last_exit;
	params.content = (*token_ptr)->content;
	set_redirection_target(cmd, token, &params);
	*token_ptr = (*token_ptr)->next;
	return (1);
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
		tokens = process_token(tokens, &parser, env, &last_exit);
	return (parser.head);
}
