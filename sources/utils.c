/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
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

char	*ft_strjoin_char(char *str, char c)
{
	char	*result;
	int		len;
	int		i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = (char *)malloc(len + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		result[i] = str[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	return (result);
}

void	exit_free(int signal,
	char **envs, t_command *cmds, t_token *tokens)
{
	free_env(envs);
	free_commands(cmds);
	free_tokens(tokens);
	exit(signal);
}

char	*get_content_value(t_content_params *params)
{
	t_token_content	*content;
	char			*temp_str;
	char			*value_str;
	char			*value_expanded;

	if (!params->content)
		return (NULL);
	value_str = ft_strdup("");
	content = params->content;
	while (content)
	{
		temp_str = value_str;
		if (content->in_single_quotes)
			value_expanded = ft_strdup(content->value);
		else
			value_expanded = expand_variable(content->value,
					params->envs, *params->last_exit_code);
		value_str = ft_strjoin(value_str, value_expanded);
		free(temp_str);
		content = content->next;
	}
	return (value_str);
}
