/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:50:10 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/04 19:31:42 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

/**
 * @brief Checks if the character is a valid variable name character
 * @param chr The character to check
 * @return 1 if valid, 0 otherwise
 * @note Valid characters are alphanumeric characters and underscore
 */
static int	is_valid_var_char(char chr)
{
	return (ft_isalnum(chr) || chr == UNDERSCORE_CHR);
}

/**
 * @brief Extracts the variable name from a string enclosed in braces
 * @param str The string to extract from
 * @param index The index to start extracting from
 * @return The extracted variable name, or NULL if an error occurs
 * @note Increments the index to point to the char after the closing brace
 *       Returns NULL if the closing brace is not found
 */
static char	*extract_braced_var_name(char *str, int *index)
{
	int		start;
	int		end;
	char	*bracket_content;

	(*index)++;
	start = *index;
	end = start;
	while (str[end] && str[end] != BRACKET_CLOSE_CHR)
		end++;
	if (!str[end])
	{
		ft_putendl_fd("minishell: syntax error: missing '}'", STDERR_FILENO);
		return (NULL);
	}
	bracket_content = ft_substr(str, start, end - start);
	*index = end + 1;
	return (bracket_content);
}

/**
 * @brief Extracts the variable name from a string enclosed in braces
 * @param str The string to extract from
 * @param index The index to start extracting from
 * @return The extracted variable name, or NULL if an error occurs
 * @note Increments the index to point to the char after the closing brace
 *       Returns NULL if empty variable name
 */
static char	*get_var_name(char *str, int *index)
{
	char	*var_name;

	var_name = extract_braced_var_name(str, index);
	if (!var_name)
		return (NULL);
	if (var_name[0] == '\0')
	{
		ft_putendl_fd("minishell: ${}: bad substitution", STDERR_FILENO);
		free(var_name);
		return (NULL);
	}
	return (var_name);
}

/**
 * @brief Extracts and validates a variable name enclosed in brackets
 * @param str The input string containing the variable reference
 * @param index Pointer to the current position in the string
 * @return The extracted variable name if valid, NULL otherwise
 * @note Validates that the variable name contains only valid characters
 *       (alphanumeric characters and underscores)
 */
char	*extract_var_name_in_brackets_entered(
	char *str, int *index)
{
	int		i;
	char	*var_name;

	var_name = get_var_name(str, index);
	if (!var_name)
		return (NULL);
	i = 0;
	while (var_name[i])
	{
		if (!is_valid_var_char(var_name[i]))
		{
			ft_putstr_fd("minishell: ${", STDERR_FILENO);
			ft_putstr_fd(var_name, STDERR_FILENO);
			ft_putendl_fd("}: bad substitution", STDERR_FILENO);
			free(var_name);
			return (NULL);
		}
		i++;
	}
	return (var_name);
}
