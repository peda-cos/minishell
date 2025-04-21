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

void	reset_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

int	handle_eof(char *input)
{
	if (!input)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (1);
	}
	return (0);
}
