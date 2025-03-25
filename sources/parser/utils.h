/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 01:33:23 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/25 02:19:22 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

/*
** Parser context structure to reduce parameter count in functions
*/
typedef struct s_parser_context
{
	t_command	*head;
	t_command	*current;
	char		**env;
	int			last_exit;
}				t_parser_context;

/*
** Appends a new command to the end of the command chain
** Returns the newly added command
*/
t_command		*append_command(t_command **head);

/*
** Processes a sequence of word tokens into command arguments
** Returns the updated token pointer
*/
t_token			*process_word_tokens(t_command *cmd, t_token *token, char **env,
					int last_exit);

#endif
