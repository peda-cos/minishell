/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:12:34 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/17 16:13:01 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Appends a line to the buffer and adds a newline character
 * @param buffer The buffer to append to
 * @param line The line to append
 * @return The new buffer with the appended line and newline character
 * @note Frees the old buffer after appending
 */
char	*append_to_buffer(char *buffer, char *line)
{
	char	*temp;
	char	*result;

	temp = buffer;
	buffer = ft_strjoin(buffer, line);
	free(temp);
	if (!buffer)
		return (NULL);
	temp = buffer;
	result = ft_strjoin(buffer, "\n");
	free(temp);
	return (result);
}

/**
 * @brief Checks if the delimiter is quoted
 * @param delim The delimiter to check
 * @return 1 if the delimiter is quoted, 0 otherwise
 * @note A quoted delimiter starts and ends with a single quote
 */
int	is_quoted_delimiter(char *delim)
{
	int	len;

	len = ft_strlen(delim);
	return (len >= 2 && delim[0] == '\'' && delim[len - 1] == '\'');
}

/**
 * @brief Processes a line from the heredoc
 * @param line The line to process
 * @param env The environment variables
 * @param last_exit The last exit status
 * @param expand_vars Flag indicating whether to expand variables
 * @return The processed line with expanded variables
 * @note Uses the expand_variable function to expand variables in the line
 */
char	*process_heredoc_line(char *line, char **env, int last_exit,
		int expand_vars)
{
	char			*expanded;
	int				has_error;
	t_expansion_ctx	expansion_cxt;

	has_error = FALSE;
	if (!expand_vars)
		return (ft_strdup(line));
	expansion_cxt.envs = env;
	expansion_cxt.last_exit = &last_exit;
	expansion_cxt.has_error_flag_control = &has_error;
	expanded = expand_variable(line, &expansion_cxt);
	return (expanded);
}

/**
 * @brief Strips the quotes from the delimiter if necessary
 * @param expand_vars Flag indicating whether to expand variables
 * @param delim The delimiter to strip
 * @return The stripped delimiter
 * @note Removes quotes from quoted delimiters when expand_vars is false
 */
char	*get_stripped_delim(int expand_vars, char *delim)
{
	char	*stripped_delim;

	if (!expand_vars && delim[0] == '\'')
	{
		stripped_delim = ft_substr(delim, 1, ft_strlen(delim) - 2);
		if (!stripped_delim)
			return (NULL);
	}
	else
		stripped_delim = ft_strdup(delim);
	if (!stripped_delim)
		return (NULL);
	return (stripped_delim);
}

/**
	* @brief Frees the input string and returns the result string
	* @param str The string to free
	* @param result The result string to return
	* @return The result string
	* @note Frees the input string after returning the result
	*/
char	*free_and_return(char *str, char *result)
{
	free(str);
	return (result);
}
