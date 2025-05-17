/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_interactive.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 19:05:27 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/17 16:15:58 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a string contains only whitespace characters
 * @param str The string to be checked
 * @return TRUE if the string is empty or
	* contains only spaces/tabs, FALSE otherwise
 * @note Returns FALSE if str is NULL
 * Iterates through each character
	* to check for non-whitespace characters
 */
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

/**
 * @brief Reads input from standard input in non-interactive mode
 * @return A dynamically allocated string
	* containing the input, or NULL on failure
 * @note Allocates memory for the input buffer
 *       Reads from standard input until EOF or error
 *       Replaces the last newline character with a null terminator
 *       Cleans up allocated resources before returning
 */
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

/**
 * @brief Processes input in non-interactive mode
 * @param env Triple pointer to the environment variables
 * @return The exit status of the last executed command
 * @note Iterates through each line of input
 *       Calls process_input to handle each line
 *       Cleans up allocated resources after processing
 *       Frees the environment variables and input string
 */
int	process_no_interactive_mode(char ***env)
{
	int		index;
	char	*input;
	char	**inputs;
	int		exit_status;

	exit_status = 0;
	input = get_input_from_no_interactive_mode();
	if (is_empty_string(input))
	{
		free_env(*env);
		free(input);
		return (0);
	}
	index = 0;
	inputs = ft_split(input, BREAKLINE_CHR);
	while (inputs[index])
	{
		if (!is_empty_string(inputs[index]))
			process_input(inputs[index], env, &exit_status);
		free(inputs[index++]);
	}
	free(inputs);
	free(input);
	free_env(*env);
	return (exit_status);
}
