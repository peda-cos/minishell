/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:54:34 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/31 20:41:24 by peda-cos         ###   ########.fr       */
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
		if (cmd->input_file)
			free(cmd->input_file);
		if (cmd->output_file)
			free(cmd->output_file);
		if (cmd->heredoc_delim)
			free(cmd->heredoc_delim);
		tmp = cmd;
		cmd = cmd->next;
		free(tmp);
	}
}

static char	*get_env_value(char *key, char **env)
{
	int		i;
	int		key_len;
	char	*value;

	if (!key || !env)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
		{
			value = ft_strdup(env[i] + key_len + 1);
			return (value);
		}
		i++;
	}
	return (ft_strdup(""));
}

static char	*handle_exit_status(int last_exit)
{
	return (ft_itoa(last_exit));
}

static char	*handle_variable(char *str, int *i, char **env, int last_exit)
{
	int		var_start;
	int		var_end;
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (handle_exit_status(last_exit));
	}
	var_start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_end = *i;
	if (var_start == var_end)
		return (ft_strdup("$"));
	var_name = ft_substr(str, var_start, var_end - var_start);
	var_value = get_env_value(var_name, env);
	free(var_name);
	return (var_value);
}

char	*expand_variables(char *str, char **env, int last_exit)
{
	char	*result;
	char	*temp;
	char	*var_value;
	int		i;
	int		in_single_quotes;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	in_single_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			in_single_quotes = !in_single_quotes;
		if (str[i] == '$' && !in_single_quotes)
		{
			var_value = handle_variable(str, &i, env, last_exit);
			temp = result;
			result = ft_strjoin(result, var_value);
			free(temp);
			free(var_value);
		}
		else
		{
			temp = result;
			result = ft_strjoin_char(result, str[i]);
			free(temp);
			i++;
		}
	}
	return (result);
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
