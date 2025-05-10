/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:54:34 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/09 00:08:24 by jlacerda         ###   ########.fr       */
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
	if (envs)
		free_env(*envs);
	if (cmds)
		free_commands(cmds);
	if (tokens)
		free_tokens(tokens);
	manager_file_descriptors(FREE, -1);
	exit(signal);
}

/**
	* @brief Manages file descriptors for the shell
	* @param fd_type Type of operation (ADD or FREE)
	* @param fd File descriptor to manage
	* @return void
	* @note Allocates memory for file descriptors and manages their lifecycle
	*/
void	manager_file_descriptors(t_manager_fd fd_type, int fd)
{
	static int	fd_index = 0;
	static int	*fd_list = NULL;

	if (fd_list == NULL)
	{
		fd_list = malloc(sizeof(int) * FD_LIST_SIZE);
		fd_index = 0;
		while (fd_index < FD_LIST_SIZE)
			fd_list[fd_index++] = -1;
		fd_index = 0;
	}
	if (fd_type == ADD)
		fd_list[fd_index++] = fd;
	else if (fd_type == FREE)
	{
		fd_index = 0;
		while (fd_list[fd_index] > STDERR_FILENO && fd_index < FD_LIST_SIZE)
		{
			close(fd_list[fd_index]);
			fd_list[fd_index] = -1;
			fd_index++;
		}
		free(fd_list);
		fd_list = NULL;
	}
}
