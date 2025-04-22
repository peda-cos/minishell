/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:15:26 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/21 23:22:07 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "macros.h"
# include "structs.h"
# include "../sources/handler/signals.h"
# include "../sources/builtin/builtin.h"
# include "../sources/expansion/expansion.h"
# include "../sources/history/history.h"
# include "../sources/executor/executor.h"

# include <fcntl.h>
# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>

/* Main Utils */
int			process_tokens(t_token **tokens, int *last_exit);
void		process_input(char *input, char ***env, int *last_exit);
void		execute_parent_builtin(
				t_command *cmd, char ***env, int *last_exit, t_token *tokens);
void		execute_parsed_commands(
				t_command *cmd, char ***env, int *last_exit, t_token *tokens);

/* Environment management */
char		**copy_env(char **envp);
void		free_env(char **env);
void		exit_free(
				int signal, char ***envs, t_command *cmds, t_token *tokens);

/* Parser: builds a linked list of commands from tokens */
t_command	*parse_tokens(t_token *tokens, char **env, int last_exit);

/* Executor: executes a command chain */
void		execute_command(
				t_command *cmd, char ***env, int *last_exit, t_token *tokens);
int			setup_output_redirection(t_process_command_args *param);

/* Memory cleanup */
void		free_tokens(t_token *tokens);
void		free_commands(t_command *cmd);

/* Utility for variable expansion */
char		*expand_variable(char *str, char **env, int last_exit);

/* Tokenizer: splits input into tokens */
t_token		*tokenize_input(char *input);

/* Validates for tokens syntax errors */
int			validate_tokens(t_token **tokens);

/* Returns a colored prompt string with rotating pastel colors */
char		*get_colored_prompt(void);

#endif
