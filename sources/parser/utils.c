/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 01:31:04 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/27 23:08:25 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"


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

t_token	*process_word_tokens(t_command *cmd, t_token *token, char **env,
		int last_exit)
{
	int	count;
	int	i;

	count = count_word_tokens(token);
	cmd->args = malloc(sizeof(char *) * (count + 1));
	if (!cmd->args)
		return (token);
	i = 0;
	while (i < count && token && token->type == WORD)
	{
		cmd->args[i] = expand_variables(token->value, env, last_exit);
		i++;
		token = token->next;
	}
	cmd->args[i] = NULL;
	cmd->argc = count;
	return (token);
}

