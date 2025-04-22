/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:54:34 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/21 23:19:54 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	free_token_content(t_token_content *content)
{
	t_token_content	*tmp;

	while (content)
	{
		tmp = content;
		content = content->next;
		free(tmp->value);
		free(tmp);
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free_token_content(tmp->content);
		free(tmp);
	}
}

void	free_commands(t_command *cmd)
{
	int			i;
	t_command	*tmp;

	while (cmd)
	{
		i = 0;
		if (cmd->args)
		{
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		free(cmd->input_file);
		free(cmd->output_file);
		free(cmd->heredoc_delim);
		if (cmd->heredoc_fd >= 0)
			close(cmd->heredoc_fd);
		tmp = cmd;
		cmd = cmd->next;
		free(tmp);
	}
}

void	exit_free(int signal, char ***envs, t_command *cmds, t_token *tokens)
{
	if (envs)
		free_env(*envs);
	if (cmds)
		free_commands(cmds);
	if (tokens)
		free_tokens(tokens);
	exit(signal);
}
