/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 01:31:04 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/02 23:25:55 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->argc = 0;
	cmd->input_file = NULL;
	cmd->output_file_list = malloc(sizeof(char *) * OUTPUT_FILES_MAX_SIZE);
	cmd->output_file_list[0] = NULL;
	cmd->amount_output_files = 0;
	cmd->was_expanded = FALSE;
	cmd->in_quotes = FALSE;
	cmd->append = 0;
	cmd->heredoc_delim = NULL;
	cmd->heredoc_fd = -1;
	cmd->next = NULL;
	return (cmd);
}

int	is_redirection(t_token_type type)
{
	return (type == REDIRECT_IN || type == REDIRECT_OUT || type == APPEND
		|| type == HEREDOC);
}

void	cleanup_parser_on_error(t_parser_context *parser)
{
	if (parser && parser->head)
	{
		free_commands(parser->head);
		parser->head = NULL;
		parser->current = NULL;
	}
}

t_command	*append_command(t_command **head)
{
	t_command	*new_cmd;
	t_command	*iter;

	new_cmd = new_command();
	if (!new_cmd)
		return (NULL);
	if (!*head)
	{
		*head = new_cmd;
		return (new_cmd);
	}
	iter = *head;
	while (iter->next)
		iter = iter->next;
	iter->next = new_cmd;
	return (new_cmd);
}

int	append_argument(t_command *cmd, char *arg_value)
{
	char	**new_args;
	int		i;

	if (!cmd || !arg_value)
		return (0);
	new_args = malloc(sizeof(char *) * (cmd->argc + 2));
	if (!new_args)
	{
		free(arg_value);
		return (0);
	}
	i = 0;
	while (i < cmd->argc)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[cmd->argc] = arg_value;
	new_args[cmd->argc + 1] = NULL;
	if (cmd->args)
		free(cmd->args);
	cmd->args = new_args;
	cmd->argc += 1;
	return (1);
}
