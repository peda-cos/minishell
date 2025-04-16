/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:54:34 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/12 22:55:58 by jlacerda         ###   ########.fr       */
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

void	free_tokens(t_token *tokens)
{
	t_token			*tmp;
	t_token_content	*tmp_content;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		while (tmp->content)
		{
			tmp_content = tmp->content;
			tmp->content = tmp->content->next;
			free(tmp_content->value);
			free(tmp_content);
		}
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
		tmp = cmd;
		cmd = cmd->next;
		free(tmp);
	}
}

void	exit_free(int signal,
	char **envs, t_command *cmds, t_token *tokens)
{
	free_env(envs);
	free_commands(cmds);
	free_tokens(tokens);
	exit(signal);
}
