/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 07:22:36 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/08 07:24:53 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
 * @brief Swaps two string pointers in an array
 * @param a Pointer to first string pointer
 * @param b Pointer to second string pointer
 * @return void
 * @note Helper function for sorting algorithm
 */
static void	swap_strings(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * @brief Extracts variable name from environment string
 * @param env_str Environment string (NAME=value or NAME)
 * @return Newly allocated string containing variable name
 * @note Caller must free the returned string
 */
static char	*extract_var_name(char *env_str)
{
	char	*equal_pos;

	equal_pos = ft_strchr(env_str, '=');
	if (equal_pos)
		return (ft_substr(env_str, 0, equal_pos - env_str));
	return (ft_strdup(env_str));
}

/**
 * @brief Compares and swaps two environment strings if needed
 * @param sorted_env Array of environment strings
 * @param j Index of first element to compare
 * @return void
 * @note Swaps elements if first is alphabetically greater than second
 */
static void	compare_and_swap(char **sorted_env, int j)
{
	char	*name1;
	char	*name2;

	name1 = extract_var_name(sorted_env[j]);
	name2 = extract_var_name(sorted_env[j + 1]);
	if (ft_strcmp(name1, name2) > 0)
		swap_strings(&sorted_env[j], &sorted_env[j + 1]);
	free(name1);
	free(name2);
}

/**
 * @brief Sorts environment variables alphabetically using bubble sort
 * @param sorted_env Array of environment variable strings to sort
 * @param count Number of elements in the array
 * @return void
 * @note Sorts in-place using variable names (before '=' if present)
 */
void	sort_env_alphabetically(char **sorted_env, int count)
{
	int	i;
	int	j;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			compare_and_swap(sorted_env, j);
			j++;
		}
		i++;
	}
}
