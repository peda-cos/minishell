/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:54:34 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 19:13:34 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	* @brief Frees the memory allocated for the environment variables
	* @param env Pointer to the environment variable array to be freed
	* @return void
	* @note Iterates through each string in the array and frees it
	*       Finally, frees the array itself
	*/
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

/**
	* @brief Frees the memory allocated for the token content
	* @param content Pointer to the token content to be freed
	* @return void
	* @note Iterates through each token content and 
	* frees its value and the content itself
	*/
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

/**
	* @brief Frees the memory allocated for the token list
	* @param tokens Pointer to the token list to be freed
	* @return void
	* @note Iterates through each token and 
	* frees its content and the token itself
	*/
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

/**
	* @brief Frees the memory allocated for the command list
	* @param cmd Pointer to the command list to be freed
	* @return void
	* @note Iterates through each command and
	* frees its arguments, input/output files,
	* heredoc delimiters, and the command itself
	*/
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
				free(cmd->args[i++]);
			free(cmd->args);
		}
		free(cmd->input_file);
		i = 0;
		while (i < cmd->amount_output_files)
			free(cmd->output_file_list[i++]);
		free(cmd->output_file_list);
		free(cmd->heredoc_delim);
		if (cmd->heredoc_fd >= 0)
			close(cmd->heredoc_fd);
		tmp = cmd;
		cmd = cmd->next;
		free(tmp);
	}
}

/**
	* @brief Frees the memory allocated for the environment variables, 
	* commands, and tokens, and exits the program with the specified signal
	* @param signal The exit signal
	* @param envs Pointer to the environment variable array to be freed
	* @param cmds Pointer to the command list to be freed
	* @param tokens Pointer to the token list to be freed
	* @return void
	* @note Cleans up all allocated resources before exiting
	*/
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
