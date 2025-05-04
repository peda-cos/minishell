/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:37:02 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/04 19:38:19 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Allocates memory for a new argument array with split words
 * @param cmd The command structure containing arguments to be processed
 * @return A newly allocated array of strings, or NULL on failure
 * @note Allocates memory for the new array based on the number of words
	*/
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

/**
	* @brief Checks if the command should be updated
	* @param cmd The command structure to check
	* @param tokens_head The head of the token list
	* @return TRUE if the command should be updated, FALSE otherwise
	* @note Checks if the command is NULL or has no arguments
	*       Checks if the first argument is "export"
	*       Checks if any token in the list is quoted
	*/
static int	should_update_cmd(t_command *cmd, t_token	*tokens_head)
{
	char	*command;
	t_token	*token;

	token = tokens_head;
	while (token)
	{
		if (token->content->in_quotes)
			return (FALSE);
		token = token->next;
	}
	if (!cmd || !cmd->args)
		return (FALSE);
	command = cmd->args[0];
	if (!command)
		return (FALSE);
	if (ft_strncmp(command, "export", 7) == 0)
		return (FALSE);
	return (TRUE);
}

/**
	* @brief Processes the split arguments and updates the new argument array
	* @param new_args The new argument array to be updated
	* @param new_idx The index to insert the new arguments
	* @param arg_splited The split arguments to be processed
	* @return TRUE on success, FALSE on failure
	* @note Allocates memory for each new argument and updates the index
	*/
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

/**
	* @brief Processes a single command argument and
	* updates the new argument array
	* @param new_args The new argument array to be updated
	* @param new_idx The index to insert the new arguments
	* @param arg The command argument to be processed
	* @return TRUE on success, FALSE on failure
	* @note Splits the argument by spaces and processes each split part
	*/
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

/**
	* @brief Updates the command arguments when expanded
	* @param cmd The command structure to be updated
	* @param tokens_head The head of the token list
	* @return void
	* @note Allocates memory for the new argument array
	*       Processes each command argument and updates the new array
	*       Frees the old argument array and assigns the new one
	*/
void	update_cmd_args_when_expanded(t_command *cmd,
	t_token	*tokens_head)
{
	int		i;
	char	**new_args;
	int		new_args_index;

	i = 0;
	new_args_index = 0;
	if (!should_update_cmd(cmd, tokens_head))
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
	new_args[new_args_index] = NULL;
	free_split(cmd->args);
	cmd->args = new_args;
}
