/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:05:27 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/26 22:17:24 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_empty_string(const char *str)
{
	int		i;

	if (str == NULL)
		return (FALSE);
	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

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
	if (bytes_read > 0 && buffer[bytes_read - 1] == '\n')
		buffer[bytes_read - 1] = '\0';
	buffer[bytes_read] = '\0';
	return (buffer);
}

static int	process_no_interactive_mode(char ***env)
{
	int		index;
	char	*input;
	char	**inputs;
	int		exit_status;

	exit_status = 0;
	input = get_input_from_no_interactive_mode();
	if (is_empty_string(input))
		return (free_env(*env), free(input), 0);
	index = 0;
	inputs = ft_split(input, '\n');
	while (inputs[index])
	{
		if (!is_empty_string(inputs[index]))
			process_input(inputs[index], env, &exit_status);
		free(inputs[index]);
		index++;
	}
	free(inputs);
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
