/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:15:26 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/15 17:48:13 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include "macros.h"
#	include "structs.h"


/* Environment management (passed locally) */
char					**copy_env(char **envp);
void					free_env(char **env);

/* Tokenizer: splits input into tokens (handling quotes and special symbols) */
t_token					*tokenize_input(char *input);

/* Parser: builds a linked list of commands from tokens */
t_command				*parse_tokens(t_token *tokens, char **env,
							int last_exit);

/* Executor: executes a command chain (with redirections/pipes) */
void					execute_command(t_command *cmd, char **env,
							int *last_exit);

/* Builtin commands */
int						is_parent_builtin(char *cmd);
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



/* Tokenizer */
// Index
t_token	*tokenize_input(char *input);
// Utils
int						is_metachar(char c);
void					add_token(t_token **tokens, t_token *new);
t_token		*new_token(char *value, t_token_type type);
int						is_quote_delimiter(char *str, int *index, char delimiter);
// Word
void	add_word_in_quotes(t_token **tokens, char *str, int *index);
void	add_word_with_quotes(t_token **tokens, char *str, int *index);
void	add_word_without_quotes(t_token **tokens, char *str, int *index);

#endif
