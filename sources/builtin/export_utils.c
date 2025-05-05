/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:37:32 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/03 15:47:55 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
 * @brief Prints an error message for an invalid option in a builtin command
 * @param arg The invalid argument
 * @return 2 if it's an invalid option, 1 for other invalid identifiers
 * @note Prints appropriate error message to standard error output
 */
int	print_invalid_arg(char *arg)
{
	if (arg[0] == DASH_CHR)
	{
		print_invalid_option("export", arg, EXPORT_OPTIONS_CHARS);
		return (2);
	}
	ft_putstr_fd("Minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (1);
}

/**
 * @brief Checks if the given key is a valid environment variable name
 * @param key The key to check
 * @return 1 if valid, 0 otherwise
 * @note Validates that the key starts with an alphabetic char or underscore
 *       and contains only alphanumeric characters or underscores
 */
int	is_valid_key(char *key)
{
	int	index;

	index = 0;
	if (!key || !*key || (!ft_isalpha(*key) && *key != UNDERSCORE_CHR))
		return (FALSE);
	while (key[index])
	{
		if (!ft_isalnum(key[index]) && key[index] != UNDERSCORE_CHR)
			return (FALSE);
		index++;
	}
	return (TRUE);
}
