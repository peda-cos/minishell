/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:54:34 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/20 22:19:01 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	* @brief Frees the input string and returns the result string
	* @param str The string to free
	* @param result The result string to return
	* @return The result string
	* @note Frees the input string after returning the result
	*/
char	*free_and_return(char *str, char *result)
{
	free(str);
	return (result);
}

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
 * @brief Frees the memory allocated for the token list
 * @param tokens Pointer to the token list to be freed
 * @return void
 * @note Iterates through each token and its content,
 *       freeing the memory allocated for each
 */
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
 * @brief Frees memory and exits program with specified signal
 * @param signal The exit signal
 * @param envs Pointer to the environment variable array to be freed
 * @param cmds Pointer to the command list to be freed
 * @param tokens Pointer to the token list to be freed
 * @return void
 * @note Cleans up all allocated resources before exiting
 */
void	exit_free(int signal, char ***envs, t_command *cmds, t_token *tokens)
{
	restore_std_file_descriptors();
	if (envs)
		free_env(*envs);
	if (cmds)
		free_commands(cmds);
	if (tokens)
		free_tokens(tokens);
	rl_clear_history();
	manager_file_descriptors(FREE, -1);
	exit(signal);
}
