/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:05:27 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/13 21:29:28 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles special cases of empty or semi-empty commands
 * @param cmd Pointer to the command structure to be processed
 * @param tokens Pointer to the token list to be freed if necessary
 * @param last_exit Pointer to the last exit status to be updated
 * @return 1 if the command was fully processed and execution should stop,
 *         0 if further processing is required
 * @note Handles three cases:
 *       1. Completely empty command (first arg is NULL_CHR and no other args)
 *       2. Command with empty first argument but has other arguments
 *       3. Invalid command (delegates to process_invalid_command)
 */
static int	process_empty_command(
	t_command *cmd, t_token *tokens, int *last_exit)
{
	int	index;

	if (cmd && cmd->args && cmd->args[0][0] == NULL_CHR
		&& cmd->args[1] == NULL && !cmd->in_quotes)
	{
		free_tokens(tokens);
		free_commands(cmd);
		*last_exit = 0;
		return (1);
	}
	if (cmd && cmd->args && cmd->args[1] && !cmd->in_quotes)
	{
		index = 0;
		free(cmd->args[0]);
		while (cmd->args[index])
		{
			cmd->args[index] = cmd->args[index + 1];
			index++;
		}
		return (0);
	}
	process_invalid_command(cmd, last_exit, tokens);
	return (1);
}

/**
 * @brief Processes user input by tokenizing, parsing, and executing commands
 * @param input The input string entered by the user
 * @param env Triple pointer to the environment variables
 * @param last_exit Pointer to the last command exit status
 * @return void
 * @note Handles the following steps:
 *       1. Tokenizes the input string into a linked list of tokens
 *       2. Processes the tokens to handle special characters
 *       3. Parses the tokens into a command structure
 *       4. Preprocesses heredocs if present in the command structure
 *       5. Sets the last argument without pipe executed
 *       6. Executes the parsed commands
 */
void	process_input(char *input, char ***env, int *last_exit)
{
	t_token		*tokens;
	t_command	*cmd;

	if (!input || !*input)
		return ;
	add_history(input);
	save_command_to_history(input);
	g_signal_received = 0;
	tokens = tokenize_input(input);
	if (process_tokens(&tokens, last_exit))
		return ;
	cmd = parse_tokens(tokens, *env, *last_exit);
	preprocess_heredocs(cmd);
	if (cmd == NULL || cmd->args == NULL || *cmd->args[0] == NULL_CHR)
	{
		if (process_empty_command(cmd, tokens, last_exit))
			return ;
	}
	set_last_arg_without_pipe_executed(tokens, cmd, env);
	execute_parsed_commands(cmd, env, last_exit, tokens);
	free_tokens(tokens);
	manager_file_descriptors(FREE, -1);
}

/**
 * @brief Main entry point for the minishell program
 * @param argc Argument count (unused but required by standard)
 * @param argv Argument vector (unused but required by standard)
 * @param envp Environment variables from the parent process
 * @return The exit status of the last executed command
 * @note The function initializes the shell environment,
 * sets up signal handling, and enters 
	* the main loop for reading user input.
 */
int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	**env;
	int		last_exit;

	(void)argc;
	(void)argv;
	last_exit = 0;
	env = copy_env(envp);
	setup_interactive_signals();
	load_history();
	if (!isatty(STDIN_FILENO))
		return (process_no_interactive_mode(&env));
	while (TRUE)
	{
		input = readline(get_colored_prompt());
		if (handle_eof(input))
			break ;
		process_input(input, &env, &last_exit);
		free(input);
	}
	free_env(env);
	return (last_exit);
}
