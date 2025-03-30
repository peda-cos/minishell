/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:54:34 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/30 02:04:18 by jlacerda         ###   ########.fr       */
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
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

void	free_commands(t_command *cmd)
{
	int			i;
	t_command	*tmp;

	if (!cmd)
		return ;
	while (cmd)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
		free(cmd->input_file);
		free(cmd->output_file);
		free(cmd->heredoc_delim);
		tmp = cmd;
		cmd = cmd->next;
		free(tmp);
	}
	free(cmd);
}

char	*expand_variables(char *str, char **env, int last_exit)
{
	char	*result;
	char	*var;
	char	*value;
	int		i;
	int		k;

	i = 0;
	if (!str || str[0] != '$')
		return (ft_strdup(str));
	if (ft_strcmp(str, "$?") == 0)
		return (ft_itoa(last_exit));
	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	var = ft_substr(str, 1, i - 1);
	k = 0;
	value = NULL;
	while (env && env[k])
	{
		if (ft_strncmp(env[k], var, ft_strlen(var)) == 0
			&& env[k][ft_strlen(var)] == '=')
		{
			value = env[k] + ft_strlen(var) + 1;
			break ;
		}
		k++;
	}
	free(var);
	if (value)
		result = ft_strdup(value);
	else
		result = ft_strdup("");
	return (result);
}
