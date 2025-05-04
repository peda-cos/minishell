/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 01:31:04 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 19:39:35 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
	* @brief Allocates memory for a new command structure
	* @return A pointer to the newly allocated
	* command structure, or NULL on failure
	* @note Initializes the command structure with default values
 */
static t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->argc = 0;
	cmd->input_file = NULL;
	cmd->output_file_list = malloc(sizeof(char *) * OUTPUT_FILES_MAX_SIZE);
	cmd->output_file_list[0] = NULL;
	cmd->amount_output_files = 0;
	cmd->was_expanded = FALSE;
	cmd->in_quotes = FALSE;
	cmd->append = 0;
	cmd->heredoc_delim = NULL;
	cmd->heredoc_fd = -1;
	cmd->next = NULL;
	return (cmd);
}

/**
	* @brief Checks if the token type is a redirection type
	* @param type The token type to check
	* @return 1 if it is a redirection type, 0 otherwise
	* @note Compares the token type with known redirection types
	*/
int	is_redirection(t_token_type type)
{
	return (type == REDIRECT_IN || type == REDIRECT_OUT || type == APPEND
		|| type == HEREDOC);
}

/**
	* @brief Cleans up the parser context on error
	* @param parser The parser context to clean up
	* @return void
	* @note Frees the command list and resets the parser context
	*/
void	cleanup_parser_on_error(t_parser_context *parser)
{
	if (parser && parser->head)
	{
		free_commands(parser->head);
		parser->head = NULL;
		parser->current = NULL;
	}
}

/**
	* @brief Appends a new command to the command list
	* @param head A pointer to the head of the command list
	* @return A pointer to the newly appended command, or NULL on failure
	* @note Allocates memory for the new command and initializes it
	*/
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

/**
	* @brief Appends an argument to the command's argument list
	* @param cmd The command structure to update
	* @param arg_value The argument value to append
	* @return 1 on success, 0 on failure
	* @note Allocates memory for the new argument and
	* appends it to the command's argument list
	*/
int	append_argument(t_command *cmd, char *arg_value)
{
	char	**new_args;
	int		i;

	if (!cmd || !arg_value)
		return (0);
	new_args = malloc(sizeof(char *) * (cmd->argc + 2));
	if (!new_args)
	{
		free(arg_value);
		return (0);
	}
	i = 0;
	while (i < cmd->argc)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[cmd->argc] = arg_value;
	new_args[cmd->argc + 1] = NULL;
	if (cmd->args)
		free(cmd->args);
	cmd->args = new_args;
	cmd->argc += 1;
	return (1);
}
