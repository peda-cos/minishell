/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 01:31:04 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/26 17:42:48 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_redirection(t_token_type type)
{
	return (type == REDIRECT_IN || type == REDIRECT_OUT || type == APPEND
		|| type == HEREDOC);
}

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
	cmd->append = 0;
	cmd->heredoc_delim = NULL;
	cmd->next = NULL;
	return (cmd);
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

static int	count_word_tokens(t_token *token)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = token;
	while (tmp && tmp->type == WORD)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

static void	free_commands_during_process_word_tokens(t_command *cmd)
{
	int	index;

	index = 0;
	while (cmd->args[index])
	{
		free(cmd->args[index]);
		index++;
	}
	free(cmd->args);
	cmd->args = NULL;
}

t_token	*process_word_tokens(t_command *cmd, t_token *token, char **env,
		int last_exit)
{
	int					i;
	int					count;
	t_content_params	content_params;

	count = count_word_tokens(token);
	cmd->args = malloc(sizeof(char *) * (count + 1));
	if (!cmd->args)
		return (token);
	i = 0;
	while (i < count && token && token->type == WORD)
	{
		content_params.envs = env;
		content_params.last_exit_code = &last_exit;
		content_params.content = token->content;
		cmd->args[i] = get_token_content_value(&content_params);
		if (!cmd->args[i])
		{
			cmd->args[i] = NULL;
			return (free_commands_during_process_word_tokens(cmd), NULL);
		}
		i++;
		token = token->next;
	}
	cmd->args[i] = NULL;
	cmd->argc = count;
	return (token);
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
