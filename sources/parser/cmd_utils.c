/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:37:02 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/28 21:21:46 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**malloc_new_args(t_command *cmd)
{
	int	words;
	int	i;

	i = 0;
	words = 0;
	if (!cmd->args || !cmd->args[0])
		return (NULL);
	while (cmd->args[i])
	{
		words += ft_count_words(cmd->args[i], SPACE_CHR);
		i++;
	}
	return ((char **)malloc(sizeof(char *) * (words + 1)));
}

static int	should_update_cmd(t_command *cmd)
{
	char	*command;

	if (!cmd || !cmd->args)
		return (FALSE);
	command = cmd->args[0];
	if (!command)
		return (FALSE);
	if (ft_strncmp(command, "export", 7) == 0)
		return (FALSE);
	return (TRUE);
}

static int	process_arg_split(char **new_args, int *new_idx, char **arg_splited)
{
	int	arg_idx;

	arg_idx = 0;
	while (arg_splited[arg_idx])
	{
		new_args[*new_idx] = ft_strdup(arg_splited[arg_idx]);
		if (!new_args[*new_idx])
			return (FALSE);
		(*new_idx)++;
		arg_idx++;
		new_args[*new_idx] = NULL;
	}
	return (TRUE);
}

static int	process_command_arg(char **new_args, int *new_idx, char *arg)
{
	char	**arg_splited;

	arg_splited = ft_split(arg, SPACE_CHR);
	if (!arg_splited || !arg_splited[0])
	{
		free_split(arg_splited);
		return (FALSE);
	}
	if (!process_arg_split(new_args, new_idx, arg_splited))
	{
		free_split(arg_splited);
		return (FALSE);
	}
	free_split(arg_splited);
	return (TRUE);
}

void	update_cmd_args_when_expanded(t_command *cmd)
{
	int		i;
	char	**new_args;
	int		new_args_index;

	i = 0;
	new_args_index = 0;
	if (!should_update_cmd(cmd))
		return ;
	new_args = malloc_new_args(cmd);
	if (!new_args)
	{
		free_commands(cmd);
		cmd = NULL;
		return ;
	}
	new_args[0] = NULL;
	while (cmd->args[i])
	{
		if (!process_command_arg(new_args, &new_args_index, cmd->args[i]))
			return (free_split(new_args));
		i++;
	}
	free_split(cmd->args);
	cmd->args = new_args;
}
