/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 01:33:23 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/21 22:21:37 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

# define SYNTAX_ERROR_MSG "minishell: syntax error near unexpected token\n"

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

/*
** Appends a single argument to a command's argument list
** Returns 1 on success, 0 on failure
*/
int				append_argument(t_command *cmd, char *arg_value);

int				is_redirection(t_token_type type);
char			*get_token_content_value(t_content_params *params);
void			free_commands(t_command *head);

#endif
