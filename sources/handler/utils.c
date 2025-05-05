/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:03:26 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/31 21:03:54 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

/**
 * @brief Resets signal handlers to their default behavior
 * @return void
 * @note Sets the signal handler for
	* SIGINT and SIGQUIT signals to SIG_DFL
 * Used to restore default signal handling
	* after special handling periods
 */
void	reset_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * @brief Handles EOF (Ctrl+D) condition from input
 * @param input The input string to check for NULL
 * @return 1 if EOF detected, 0 otherwise
 * @note Displays "exit" message if input is NULL and running in terminal
 *       Used to detect end of input in interactive mode
 */
int	handle_eof(char *input)
{
	if (!input)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "exit\n", 5);
		return (1);
	}
	return (0);
}
