/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:20:01 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 19:28:29 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
	* @brief Checks if the argument is a valid -n option for echo
	* @param arg The argument to check
	* @return 1 if valid, 0 otherwise
	* @note Validates that the argument starts
	* with -n and contains only 'n' characters
	*/
static int	is_valid_n_option(char *arg)
{
	int	j;

	j = 1;
	if (!ft_strncmp(arg, "-n", 2))
	{
		while (arg[j])
		{
			if (arg[j] != 'n')
				return (0);
			j++;
		}
		return (1);
	}
	return (0);
}

/**
	* @brief Prints the arguments for the echo command
	* @param args The arguments to print
	* @param start_idx The index to start printing from
	* @return void
	* @note Prints each argument followed by a space, except for the last one
	*/
static void	print_echo_args(char **args, int start_idx)
{
	int	i;

	i = start_idx;
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
}

/**
	* @brief Built-in echo command implementation
	* @param args The command-line arguments
	* @param last_exit Pointer to the last exit status
	* @return 0 on success, 1 on failure
	* @note Handles the -n option to suppress the trailing newline
	*/
int	builtin_echo(char **args, int *last_exit)
{
	int	i;
	int	newline;

	(void)last_exit;
	if (!args || !*args)
		return (1);
	i = 1;
	newline = 1;
	while (args[i] && is_valid_n_option(args[i]))
	{
		newline = 0;
		i++;
	}
	print_echo_args(args, i);
	if (newline)
		ft_putendl_fd("", STDOUT_FILENO);
	return (0);
}
