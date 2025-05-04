/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:05:27 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/03 17:09:06 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (cmd == NULL || cmd->args == NULL || *cmd->args[0] == NULL_CHR)
	{
		if (process_empty_command(cmd, tokens, last_exit))
			return ;
	}
	preprocess_heredocs(cmd);
	set_last_arg_without_pipe_executed(tokens, cmd, env);
	execute_parsed_commands(cmd, env, last_exit, tokens);
	free_tokens(tokens);
}

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
