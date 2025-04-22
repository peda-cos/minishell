/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:40:39 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/21 22:21:36 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	set_redirection_target(t_command *cmd, t_token *token,
		t_content_params *content_params)
{
	char	*value;

	value = get_token_content_value(content_params);
	if (!value)
		return ;
	if (token->type == REDIRECT_IN)
	{
		free(cmd->input_file);
		cmd->input_file = value;
	}
	else if (token->type == REDIRECT_OUT)
	{
		free(cmd->output_file);
		cmd->output_file = value;
		cmd->append = 0;
	}
	else if (token->type == APPEND)
	{
		free(cmd->output_file);
		cmd->output_file = value;
		cmd->append = 1;
	}
	else if (token->type == HEREDOC)
	{
		free(cmd->heredoc_delim);
		cmd->heredoc_delim = value;
	}
	else
		free(value);
}

static int	create_output_file(char *filename, int append)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		perror(filename);
		return (0);
	}
	close(fd);
	return (1);
}

static int	handle_redirection(t_command *cmd, t_token **token_ptr, char **envs,
		int *last_exit)
{
	t_token				*token;
	int					result;
	t_content_params	params;
	char				*content_value;

	token = *token_ptr;
	*token_ptr = token->next;
	if (!*token_ptr || (*token_ptr)->type != WORD)
		return (ft_putstr_fd(SYNTAX_ERROR_MSG, STDERR_FILENO), 0);
	params.envs = envs;
	params.last_exit_code = last_exit;
	params.content = (*token_ptr)->content;
	if (token->type == REDIRECT_OUT || token->type == APPEND)
	{
		content_value = get_token_content_value(&params);
		if (!content_value)
			return (0);
		result = create_output_file(content_value, token->type == APPEND);
		free(content_value);
		if (!result)
			return (0);
	}
	set_redirection_target(cmd, token, &params);
	*token_ptr = (*token_ptr)->next;
	return (1);
}

static void	cleanup_parser_on_error(t_parser_context *parser)
{
	if (parser && parser->head)
	{
		free_commands(parser->head);
		parser->head = NULL;
		parser->current = NULL;
	}
}

static t_token	*process_token(t_token *token, t_parser_context *parser,
		char **envs, int *last_exit)
{
	t_content_params	content_params;
	char				*arg_val;

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
	{
		content_params.envs = envs;
		content_params.last_exit_code = last_exit;
		content_params.content = token->content;
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
		return (token->next);
	}
	return (token->next);
}

t_command	*parse_tokens(t_token *tokens, char **env, int last_exit)
{
	t_parser_context	parser;
	t_token				*next_token;

	parser.head = NULL;
	parser.current = NULL;
	parser.env = env;
	parser.last_exit = last_exit;
	while (tokens)
	{
		next_token = process_token(tokens, &parser, env, &last_exit);
		if (!next_token && !parser.head)
			return (NULL);
		tokens = next_token;
	}
	return (parser.head);
}
