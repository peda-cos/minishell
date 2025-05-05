/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:56:00 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 19:00:28 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

/**
	* Global signal flag to indicate which signal was received
	* Used to communicate signal events between signal handlers and main code
	*/
volatile sig_atomic_t	g_signal_received = 0;

/**
 * @brief Handles the SIGINT signal in interactive mode
 * @param signo The signal number
 * @note Sets the global signal flag and writes a newline to stdout
 *       Resets the readline line and redisplays the prompt
 */
static void	interactive_sigint_handler(int signo)
{
	(void)signo;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Handles the SIGINT and SIGQUIT signals during command execution
 * @param signo The signal number
 * @note Sets the global signal flag and writes a message to stdout
 *       Displays a newline for SIGINT and "Quit (core dumped)" for SIGQUIT
 */
static void	execution_signal_handler(int signo)
{
	g_signal_received = signo;
	if (signo == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	else if (signo == SIGQUIT)
		write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
}

/**
 * @brief Sets up signal handlers for interactive mode
 * @return void
 * @note Configures SIGINT to be handled by interactive_sigint_handler
 *       Configures SIGQUIT to be ignored in interactive mode
 */
void	setup_interactive_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_int.sa_handler = &interactive_sigint_handler;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/**
 * @brief Sets up signal handlers for command execution
 * @return void
 * @note Configures both SIGINT and SIGQUIT
	* to be handled by execution_signal_handler
 */
void	setup_execution_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_int.sa_handler = &execution_signal_handler;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sa_quit.sa_handler = &execution_signal_handler;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
