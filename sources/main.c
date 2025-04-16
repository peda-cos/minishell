/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:05:27 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/12 22:57:25 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_parent_builtin(t_command *cmd, char ***env, int *last_exit,
		t_token *tokens)
{
	if (!ft_strcmp(cmd->args[0], "cd"))
		*last_exit = builtin_cd(cmd->args, env);
	else if (!ft_strcmp(cmd->args[0], "export"))
		*last_exit = builtin_export(cmd->args, env);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		*last_exit = builtin_unset(cmd->args, env);
	else if (!ft_strcmp(cmd->args[0], "exit"))
	{
		*last_exit = builtin_exit(cmd->args);
		exit_free(*last_exit, *env, cmd, tokens);
	}
}

static void	execute_command_line(t_command *cmd, char **env, int *last_exit,
		t_token *tokens)
{
	int	saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	setup_execution_signals();
	execute_command(cmd, env, last_exit, tokens);
	setup_interactive_signals();
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}

static int	process_tokens(t_token **tokens, int *last_exit)
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

static void	execute_parsed_commands(t_command *cmd, char ***env, int *last_exit,
		t_token *tokens)
{
	if (!cmd)
		return ;
	if (cmd->args && !cmd->next && is_parent_builtin(cmd->args[0]))
		execute_parent_builtin(cmd, env, last_exit, tokens);
	else
		execute_command_line(cmd, *env, last_exit, tokens);
	free_commands(cmd);
}

static void	process_input(char *input, char ***env, int *last_exit)
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
