/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:40:39 by peda-cos          #+#    #+#             */
/*   Updated: 2025/02/26 17:58:36 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == WORD)
			count++;
		tokens = tokens->next;
	}
	return (count);
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
	cmd->output_file = NULL;
	cmd->append = 0;
	cmd->heredoc_delim = NULL;
	cmd->next = NULL;
	return (cmd);
}

t_command	*parse_tokens(t_token *tokens)
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
			if (tokens && tokens->type == WORD)
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
				current->args[i] = expand_variables(tokens->value, NULL, 0);
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
