/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 01:33:23 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 19:48:43 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

# define OUTPUT_FILES_MAX_SIZE 25
# define SYNTAX_ERROR_MSG "minishell: syntax error near unexpected token\n"

/**
 * @brief Structure maintaining the context for parsing commands
 * @struct s_parser_context
 * 
 * Members:
 * - head: Pointer to the first command in the command list
 * - current: Pointer to the command currently being processed
 * - env: Environment variables array used for variable expansion
 * - last_exit: The exit status of the last executed command
 * - was_expanded: Flag indicating if variable expansion occurred during parsing
 * 
 * @note Used to track parser state during the command parsing process
 *       Provides central access to command list, 
 *		 expansion state, and environment
 */
typedef struct s_parser_context
{
	t_command	*head;
	t_command	*current;
	char		**env;
	int			last_exit;
	int			was_expanded;
}				t_parser_context;

void			free_commands(t_command *head);
int				is_redirection(t_token_type type);
t_command		*append_command(t_command **head);
int				append_argument(t_command *cmd, char *arg_value);
void			cleanup_parser_on_error(t_parser_context *parser);
char			*get_token_content_value(t_content_params *params);
void			update_cmd_args_when_expanded(t_command *cmd,
					t_token *tokens_head);
void			set_output_redirect_file(char *value,
					t_command	*cmd, int is_append);
void			set_redirection_target(t_command *cmd,
					t_token *token, t_content_params *content_params);
int				handle_redirection(t_command *cmd,
					t_token **token_ptr, char **envs, int *last_exit);

#endif
