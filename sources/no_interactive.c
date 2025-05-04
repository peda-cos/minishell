/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_interactive.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 19:05:27 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/03 16:42:19 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_empty_string(const char *str)
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

int	process_no_interactive_mode(char ***env)
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
