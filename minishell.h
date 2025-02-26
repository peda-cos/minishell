/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:15:26 by peda-cos          #+#    #+#             */
/*   Updated: 2025/02/26 17:55:19 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

/* Token types */
typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	APPEND
}						t_token_type;

/* Token linked list */
typedef struct s_token
{
	char				*value;
	t_token_type		type;
	struct s_token		*next;
}						t_token;

/* Command structure: a command is one segment (separated by pipes) */
typedef struct s_command
{
	char				**args;
	int					argc;
	char				*input_file;
	char				*output_file;
	int					append;
	char				*heredoc_delim;
	struct s_command	*next;
}						t_command;

/* Environment management (passed locally) */
char					**copy_env(char **envp);
void					free_env(char **env);

/* Tokenizer: splits input into tokens (handling quotes and special symbols) */
t_token					*tokenize_input(char *input);

/* Parser: builds a linked list of commands from tokens */
t_command				*parse_tokens(t_token *tokens);

/* Executor: executes a command chain (with redirections/pipes) */
void					execute_command(t_command *cmd, char **env,
							int *last_exit);

/* Builtin commands */
int						builtin_echo(char **args, int *last_exit);
int						builtin_cd(char **args);
int						builtin_pwd(void);
int						builtin_export(char **args, char ***env);
int						builtin_unset(char **args, char ***env);
int						builtin_env(char **env);
int						builtin_exit(char **args);

/* Signal handling */
void					signal_handler(int signo);

/* Memory cleanup */
void					free_tokens(t_token *tokens);
void					free_commands(t_command *cmd);

/* Utility for variable expansion */
char					*expand_variables(char *str, char **env, int last_exit);

#endif
