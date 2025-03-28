/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:40:39 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/27 23:34:23 by jlacerda         ###   ########.fr       */
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

int	main(void)
{
	t_command	*cmd;
	int	last_exit;

	char *mock_env[] = {
			"PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki",
			"HOME=/Users/jlacerda",
			"USER=jlacerda",
			"LOGNAME=jlacerda",
			"SHLVL=1",
			"PWD=/Users/jlacerda",
			"OLDPWD=/Users/jlacerda",
			"_=/usr/bin/env",
			NULL // Termina com NULL para indicar o fim da lista
	};
	// t_token token3 = { .value = "file.txt", .type = WORD, .next = NULL };
	t_token token2 = { .value = "-al", .type = REDIRECT_OUT, .next = NULL };
	t_token token1 = { .value = "ls", .type = WORD, .next = &token2 };

	t_token *tokens = &token1;
	last_exit = 0;
	cmd = parse_tokens(tokens, mock_env, last_exit);
	t_command	*tmp = NULL;
	while (cmd)
	{
		tmp = cmd;
		printf("Command:\n");
		printf("Args:\n");
		for (int i = 0; cmd->args[i]; i++)
		{
			printf("  %s\n", cmd->args[i]);
			free(cmd->args[i]);
		}
		free(cmd->args);
		free(cmd->input_file);
		free(cmd->output_file);
		free(cmd->heredoc_delim);
		printf("Input file: %s\n", cmd->input_file);
		printf("Output file: %s\n", cmd->output_file);
		printf("Append: %d\n", cmd->append);
		printf("Heredoc delimiter: %s\n", cmd->heredoc_delim);
		cmd = cmd->next;
		free(tmp);
	}
	return (0);
}
