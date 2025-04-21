/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:05:27 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/21 17:51:37 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_input_from_no_interactive_mode(void)
{
	char	*buffer;
	ssize_t	bytes_read;

	buffer = malloc(4096);
	if (!buffer)
		return (NULL);
	bytes_read = read(STDIN_FILENO, buffer, 4095);
	if (bytes_read < 0)
	{
		free(buffer);
		return (NULL);
	}
	if (buffer[bytes_read - 1] == '\n')
		buffer[bytes_read - 1] = '\0';
	buffer[bytes_read] = '\0';
	return (buffer);
}

static int	process_no_interactive_mode(char ***env)
{
	char	*input;
	int		exit_status;

	input = get_input_from_no_interactive_mode();
	process_input(input, env, &exit_status);
	free(input);
	free_env(*env);
	return (exit_status);
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
