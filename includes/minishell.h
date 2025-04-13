/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:15:26 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/12 23:31:13 by jlacerda         ###   ########.fr       */
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

# include <fcntl.h>
# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>

/* Environment management */
char		**copy_env(char **envp);
void		free_env(char **env);
void		exit_free(int signal,
				char **envs, t_command *cmds, t_token *tokens);

/* Parser: builds a linked list of commands from tokens */
t_command	*parse_tokens(t_token *tokens, char **env, int last_exit);

/* Executor: executes a command chain */
void		execute_command(t_command *cmd, char **env, int *last_exit,
				t_token *tokens);

/* Signal handling */
void		signal_handler(int signo);

/* Memory cleanup */
void		free_tokens(t_token *tokens);
void		free_commands(t_command *cmd);

/* Utility for variable expansion */
char		*expand_variable(char *str, char **env, int last_exit);
char		*ft_strjoin_char(char *str, char c);

/* Tokenizer: splits input into tokens */
t_token		*tokenize_input(char *input);

/* Validates for tokens syntax errors */
int			validate_tokens(t_token **tokens, char **env, int *last_exit);

/* Utils */
char		*get_content_value(t_content_params *params);

#endif
