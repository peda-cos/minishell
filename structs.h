/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 22:48:48 by jlacerda          #+#    #+#             */
/*   Updated: 2025/03/22 15:50:50 by jlacerda         ###   ########.fr       */
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
	REDIRECT_OUT
}	t_token_type;

/* Token linked list */
typedef struct s_token
{
	t_token_type	type;
	struct s_token	*next;
	char			*value;
}	t_token;

/* Command structure: a command is one segment (separated by pipes) */
typedef struct s_command
{
	int					argc;
	int					append;
	struct s_command	*next;
	char				**args;
	char				*input_file;
	char				*output_file;
	char				*heredoc_delim;
}						t_command;

#endif
