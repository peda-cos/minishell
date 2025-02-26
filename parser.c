/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:40:39 by peda-cos          #+#    #+#             */
/*   Updated: 2025/02/26 14:49:29 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmd;
	char		**args;
	int			count;
	int			i;
	t_token		*tmp;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	count = count_tokens(tokens);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
	{
		free(cmd);
		return (NULL);
	}
	i = 0;
	tmp = tokens;
	while (tmp)
	{
		args[i] = ft_strdup(tmp->value);
		i++;
		tmp = tmp->next;
	}
	args[i] = NULL;
	cmd->args = args;
	cmd->argc = count;
	cmd->next = NULL;
	return (cmd);
}
