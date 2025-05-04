/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 22:48:48 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/28 21:01:27 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

/* Token types */
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

/* Token linked list */
typedef struct s_token
{
	t_token_type			type;
	struct s_token			*prev;
	struct s_token			*next;
	t_token_content			*content;
	int						length;
}							t_token;

/* Command structure: a command is one segment (separated by pipes) */
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

typedef struct s_process_command_args
{
	int						pid;
	char					***env;
	t_command				*cmd;
	int						pipefd[2];
	int						*last_exit;
	t_token					*tokens;
	int						has_fd_redirect_to_stderr;
}							t_process_command_args;

typedef struct s_expansion_ctx
{
	char			**envs;
	t_token_content	*content;
	int				*last_exit;
	int				*was_expanded;
	int				*has_error_flag_control;
}	t_expansion_ctx;

#endif
