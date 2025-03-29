/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 15:18:06 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/29 15:40:10 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"

/**
 * Global signal flag to indicate which signal was received
 * Used to communicate signal events between signal handlers and main code
 */
extern volatile __sig_atomic_t	g_signal_received;

/**
 * Setup signal handling for interactive mode (prompt)
 */
void							setup_interactive_signals(void);

/**
 * Setup signal handling for command execution mode
 */
void							setup_execution_signals(void);

/**
 * Reset signals to default behavior
 */
void							reset_signals(void);

/**
 * Handle EOF (Ctrl+D) in interactive mode
 * Returns 1 if shell should exit, 0 otherwise
 */
int								handle_eof(char *input);

#endif