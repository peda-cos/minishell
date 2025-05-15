/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 22:48:48 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/14 22:10:44 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

/**
 * @brief Enumeration of file descriptor management options
 * @enum t_manager_fd
 */
typedef enum e_manager_fd
{
	ADD,
	FREE,
}	t_manager_fd;

/**
 * @brief Enumeration of token types supported by the minishell parser
 * @enum t_token_type
 * 
 * Values:
 * - WORD: Regular text or command arguments
 * - PIPE: Pipeline character (|) connecting commands
 * - APPEND: Output append redirection (>>)
 * - HEREDOC: Heredoc input redirection (<<)
 * - REDIRECT_IN: Input redirection (<)
 * - REDIRECT_OUT: Output redirection (>)
 * - FILE_DESCRIPTOR: File descriptor number for redirections
 * 
 * @note Used to categorize tokens during lexical analysis and parsing
 */
typedef enum e_token_type
{
	WORD,
	PIPE,
	APPEND,
	HEREDOC,
	REDIRECT_IN,
	REDIRECT_OUT,
	FILE_DESCRIPTOR,
}							t_token_type;

/**
	* @brief Structure representing the content of a token
	* @struct s_token_content
	* 
	* Members:
	* - next: Pointer to the next token content in the linked list
	* - value: The string value of the token content
	* - in_quotes: Flag indicating if the content is in double quotes
	* - in_single_quotes: Flag indicating if the content is in single quotes
	*/
typedef struct s_token_content
{
	struct s_token_content	*next;
	char					*value;
	int						in_quotes;
	int						in_single_quotes;
}							t_token_content;

typedef struct s_content_params
{
	char					**envs;
	t_token_content			*content;
	int						was_expanded;
	int						*last_exit_code;
}							t_content_params;

/**
	* @brief Structure representing a token in the linked list
	* @struct s_token
	* 
	* Members:
	* - type: The type of the token (e.g., WORD, PIPE, etc.)
	* - prev: Pointer to the previous token in the linked list
	* - next: Pointer to the next token in the linked list
	* - content: Pointer to the content of the token
	* - length: Length of the token content
	*/
typedef struct s_token
{
	t_token_type			type;
	struct s_token			*prev;
	struct s_token			*next;
	t_token_content			*content;
	int						length;
}							t_token;

/**
	* @brief Structure representing a command in the command list
	* @struct s_command
	* 
	* Members:
	* - argc: Number of arguments in the command
	* - append: Flag indicating if output should be appended
	* - next: Pointer to the next command in the linked list
	* - args: Array of command arguments
	* - input_file: Input file for redirection
	* - output_file_list: List of output files for redirection
	* - amount_output_files: Number of output files in the list
	* - heredoc_delim: Delimiter for heredoc input
	* - heredoc_fd: File descriptor for heredoc input
	* - was_expanded: Flag indicating if the command was expanded
	* - in_quotes: Flag indicating if the command is in quotes
	*/
typedef struct s_command
{
	int						argc;
	int						append;
	struct s_command		*next;
	char					**args;
	char					*input_file;
	char					**output_file_list;
	int						amount_output_files;
	char					*heredoc_delim;
	int						heredoc_fd;
	int						was_expanded;
	int						in_quotes;
}							t_command;

/**
	* @brief Structure representing the context of the parser
	* @struct s_parser_context
	* 
	* Members:
	* - env: Array of environment variables
	* - head: Pointer to the head of the command list
	* - current: Pointer to the current command being processed
	* - was_expanded: Flag indicating if the command was expanded
	* - last_exit: Pointer to the last exit status
	*/
typedef struct s_process_command_args
{
	int						pid;
	char					***env;
	t_command				*cmd;
	t_command				*head;
	int						pipefd[2];
	int						*last_exit;
	t_token					*tokens;
	int						has_fd_redirect_to_stderr;
	int						has_error_flag_control;
}							t_process_command_args;

/**
	* @brief Structure representing the context of variable expansion
	* @struct s_expansion_ctx
	* 
	* Members:
	* - envs: Array of environment variables
	* - content: Pointer to the token content being expanded
	* - last_exit: Pointer to the last exit status
	* - was_expanded: Flag indicating if the variable was expanded
	* - has_error_flag_control: Flag indicating
	* if an error occurred during expansion
	*/
typedef struct s_expansion_ctx
{
	char			**envs;
	t_token_content	*content;
	int				*last_exit;
	int				*was_expanded;
	int				*has_error_flag_control;
}	t_expansion_ctx;

#endif
