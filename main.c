/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:05:27 by peda-cos          #+#    #+#             */
/*   Updated: 2025/02/26 16:07:53 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_token		*tokens;
	t_command	*cmd;
	char		**env;

	(void)argc;
	(void)argv;
	env = copy_env(envp);
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
			cmd = parse_tokens(tokens);
			if (cmd)
				execute_command(cmd, env);
			free_commands(cmd);
			free_tokens(tokens);
		}
		free(input);
	}
	free_env(env);
	return (0);
}
