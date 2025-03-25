/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:40:39 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/22 18:44:27 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->argc = 0;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append = 0;
	cmd->heredoc_delim = NULL;
	cmd->next = NULL;
	return (cmd);
}

t_command	*parse_tokens(t_token *tokens, char **env, int last_exit)
{
	t_command	*head;
	t_command	*current;
	t_token		*tmp;
	int			count;
	int			i;
	t_command	*iter;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		if (!current)
		{
			current = new_command();
			if (!head)
				head = current;
			else
			{
				iter = head;
				while (iter->next)
					iter = iter->next;
				iter->next = current;
			}
		}
		if (tokens->type == PIPE)
		{
			current = NULL;
			tokens = tokens->next;
			continue ;
		}
		if (tokens->type == REDIRECT_IN)
		{
			tokens = tokens->next;
			if (!tokens || tokens->type != WORD)
			{
				fprintf(stderr,
					"minishell: syntax error near unexpected token\n");
				return (NULL);
			}
			current->input_file = ft_strdup(tokens->value);
		}
		else if (tokens->type == REDIRECT_OUT)
		{
			tokens = tokens->next;
			if (tokens && tokens->type == WORD)
			{
				current->output_file = ft_strdup(tokens->value);
				current->append = 0;
			}
		}
		else if (tokens->type == APPEND)
		{
			tokens = tokens->next;
			if (tokens && tokens->type == WORD)
			{
				current->output_file = ft_strdup(tokens->value);
				current->append = 1;
			}
		}
		else if (tokens->type == HEREDOC)
		{
			tokens = tokens->next;
			if (tokens && tokens->type == WORD)
				current->heredoc_delim = ft_strdup(tokens->value);
		}
		else if (tokens->type == WORD)
		{
			tmp = tokens;
			count = 0;
			while (tmp && tmp->type == WORD)
			{
				count++;
				tmp = tmp->next;
				if (tmp && tmp->type != WORD)
					break ;
			}
			current->args = malloc(sizeof(char *) * (count + 1));
			if (!current->args)
				return (head);
			i = 0;
			while (i < count && tokens && tokens->type == WORD)
			{
				current->args[i] = expand_variables(tokens->value, env,
						last_exit);
				i++;
				tokens = tokens->next;
			}
			current->args[i] = NULL;
			current->argc = count;
			continue ;
		}
		tokens = tokens->next;
	}
	return (head);
}
