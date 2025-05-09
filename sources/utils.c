/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:05:27 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/08 22:02:49 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Sets the value of the underscore argument in the command structure
 * @param cmd Pointer to the command structure
 * @param envs Triple pointer to the environment variables
 * @return void
 * @note Sets the value of the underscore
	* argument to the last command's exit status
 */
void	set_last_arg_without_pipe_executed(
	t_token *tokens, t_command *cmd, char ***envs)
{
	t_token	*current_token;

	if (!tokens || !cmd || !envs)
		return ;
	current_token = tokens;
	while (current_token && current_token->type != PIPE)
		current_token = current_token->next;
	if (current_token && current_token->type == PIPE)
		return ;
	set_underscore_arg_value(cmd, envs);
}

/**
 * @brief Executes built-in commands in the parent process
 * @param cmd Pointer to the command structure to be executed
 * @param env Triple pointer to the environment variables
 * @param last_exit Pointer to the last exit status to be updated
 * @param tokens Pointer to the token list for cleanup during exit
 * @return void
 * @note Handles built-in commands like cd, export, unset, and exit
 */
void	execute_parent_builtin(t_command *cmd, char ***env, int *last_exit,
		t_token *tokens)
{
	t_process_command_args	args;
	char					*command;
	char					**command_with_args;

	args.cmd = cmd;
	args.env = env;
	args.tokens = tokens;
	args.last_exit = last_exit;
	args.has_fd_redirect_to_stderr = FALSE;
	setup_output_redirection(&args);
	command = cmd->args[0];
	command_with_args = cmd->args;
	if (!ft_strcmp(command, "cd"))
		*last_exit = builtin_cd(command_with_args, env);
	else if (!ft_strcmp(command, "export"))
		*last_exit = builtin_export(command_with_args, env);
	else if (!ft_strcmp(command, "unset"))
		*last_exit = builtin_unset(command_with_args, env);
	else if (!ft_strcmp(command, "exit"))
	{
		*last_exit = builtin_exit(&args);
		if (*last_exit != 1)
			exit_free(*last_exit, env, cmd, tokens);
	}
}

/**
 * @brief Processes the tokens and validates them
 * @param tokens Pointer to the token list to be processed
 * @param last_exit Pointer to the last exit status to be updated
 * @return int Returns 1 if there are errors, 0 otherwise
 * @note Validates the tokens for
	* syntax errors and sets the last exit status
 */
int	process_tokens(t_token **tokens, int *last_exit)
{
	if (!*tokens)
		return (1);
	if (validate_tokens(tokens))
	{
		*last_exit = STDERR_FILENO;
		free_tokens(*tokens);
		return (1);
	}
	return (0);
}

/**
 * @brief Executes the parsed commands and handles redirection
 * @param cmd Pointer to the command structure to be executed
 * @param env Triple pointer to the environment variables
 * @param last_exit Pointer to the last exit status to be updated
 * @param tokens Pointer to the token list for cleanup during exit
 * @return void
 * @note Sets up signal handling and restores
	* standard file descriptors after execution
 */
void	execute_parsed_commands(t_command *cmd, char ***env, int *last_exit,
		t_token *tokens)
{
	setup_execution_signals();
	if (!cmd)
		return ;
	if (cmd->args && !cmd->next && is_parent_builtin(cmd->args[0]))
		execute_parent_builtin(cmd, env, last_exit, tokens);
	else
		execute_command(cmd, env, last_exit, tokens);
	setup_interactive_signals();
	free_commands(cmd);
}

/**
 * @brief Processes invalid commands and sets the last exit status
 * @param cmd Pointer to the command structure to be processed
 * @param last_exit Pointer to the last exit status to be updated
 * @param tokens Pointer to the token list for cleanup during exit
 * @return void
 * @note Handles cases where the command is empty or invalid
 */
void	process_invalid_command(t_command *cmd,
	int *last_exit, t_token *tokens)
{
	if (cmd && cmd->args && *cmd->args[0] == '\0')
	{
		if (!cmd->was_expanded || cmd->in_quotes)
			ft_putendl_fd("Minishell: Command  '' not found", STDERR_FILENO);
		*last_exit = 127;
	}
	else
		*last_exit = 1;
	free_commands(cmd);
	free_tokens(tokens);
}
