/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:05:27 by peda-cos          #+#    #+#             */
/*   Updated: 2025/02/27 13:42:55 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_token		*tokens;
	t_command	*cmd;
	char		**env;
	int			last_exit;
	int			saved_stdin;

	(void)argc;
	(void)argv;
	last_exit = 0;
	env = copy_env(envp);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		tokens = tokenize_input(input);
		if (tokens)
		{
			cmd = parse_tokens(tokens, env, last_exit);
			if (cmd)
			{
				if (!cmd->next && is_parent_builtin(cmd->args[0]))
				{
					if (!ft_strcmp(cmd->args[0], "cd"))
						last_exit = builtin_cd(cmd->args);
					else if (!ft_strcmp(cmd->args[0], "export"))
						last_exit = builtin_export(cmd->args, &env);
					else if (!ft_strcmp(cmd->args[0], "unset"))
						last_exit = builtin_unset(cmd->args, &env);
					else if (!ft_strcmp(cmd->args[0], "exit"))
						last_exit = builtin_exit(cmd->args);
					free_commands(cmd);
				}
				else
				{
					saved_stdin = dup(STDIN_FILENO);
					execute_command(cmd, env, &last_exit);
					dup2(saved_stdin, STDIN_FILENO);
					close(saved_stdin);
					free_commands(cmd);
				}
			}
			free_tokens(tokens);
		}
		free(input);
	}
	free_env(env);
	return (0);
}
