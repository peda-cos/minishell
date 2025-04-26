/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 22:48:48 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/26 17:43:07 by jlacerda         ###   ########.fr       */
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
	int						in_single_quotes;
}							t_token_content;

typedef struct s_content_params
{
	char					**envs;
	int						*last_exit_code;
	t_token_content			*content;
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

#endif
