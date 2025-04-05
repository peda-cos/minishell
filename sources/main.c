/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:05:27 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/05 19:37:02 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_parent_builtin(t_command *cmd, char ***env, int *last_exit)
{
	if (!ft_strcmp(cmd->args[0], "cd"))
		*last_exit = builtin_cd(cmd->args, env);
	else if (!ft_strcmp(cmd->args[0], "export"))
		*last_exit = builtin_export(cmd->args, env);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		*last_exit = builtin_unset(cmd->args, env);
	else if (!ft_strcmp(cmd->args[0], "exit"))
		*last_exit = builtin_exit(cmd->args);
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

static void	process_input(char *input, char ***env, int *last_exit)
{
	t_token		*tokens;
	t_command	*cmd;

	if (!input || !*input)
		return ;
	add_history(input);
	g_signal_received = 0;
	tokens = tokenize_input(input);
	if (!tokens)
		return ;
	if (validate_tokens(&tokens))
	{
		*last_exit = STDERR_FILENO;
		return (free_tokens(tokens));
	}
	cmd = parse_tokens(tokens, *env, *last_exit);
	if (cmd)
	{
		if (!cmd->next && is_parent_builtin(cmd->args[0]))
			execute_parent_builtin(cmd, env, last_exit);
		else
			execute_command_line(cmd, *env, last_exit, tokens);
		free_commands(cmd);
	}
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
	while (1)
	{
		input = readline("minishell$ ");
		if (handle_eof(input))
			break ;
		process_input(input, &env, &last_exit);
		free(input);
	}
	free_env(env);
	return (last_exit);
}
