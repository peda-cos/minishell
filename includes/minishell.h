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

# include "libft.h"
# include "macros.h"
# include "structs.h"
# include "../sources/handler/signals.h"

# include <fcntl.h>
# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>

/*			Environment management (passed locally) */
char		**copy_env(char **envp);
void		free_env(char **env);

/* 			Parser: builds a linked list of commands from tokens */
t_command	*parse_tokens(t_token *tokens, char **env, int last_exit);

/* 			Executor: executes a command chain (with redirections/pipes) */
void		execute_command(t_command *cmd, char **env, int *last_exit);

/*			Builtin commands */
int			builtin_pwd(void);
int			builtin_env(char **env);
int			builtin_cd(char **args);
int			builtin_exit(char **args);
int			is_parent_builtin(char *cmd);
int			builtin_unset(char **args, char ***env);
int			builtin_export(char **args, char ***env);
int			builtin_echo(char **args, int *last_exit);

/* 			Signal handling */
void		signal_handler(int signo);

/* 			Memory cleanup */
void		free_tokens(t_token *tokens);
void		free_commands(t_command *cmd);

/* 			Utility for variable expansion */
char		*expand_variables(char *str, char **env, int last_exit);

/* 			Tokenizer: splits input into tokens 
			(handling quotes and special symbols) */
int			is_metachar(char c);
t_token		*tokenize_input(char *input);
void		add_token(t_token **tokens, t_token *new);
t_token		*new_token(char *value, t_token_type type);
int			is_quote_delimiter(char *str, int *index, char delimiter);
void		add_word_in_quotes(t_token **tokens, char *str, int *index);
void		add_word_with_quotes(t_token **tokens, char *str, int *index);
void		add_word_without_quotes(t_token **tokens, char *str, int *index);

#endif
