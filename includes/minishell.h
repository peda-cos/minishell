/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:15:26 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/31 16:43:08 by peda-cos         ###   ########.fr       */
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
char	**copy_env(char **envp);
void	free_env(char **env);

/* 			Parser: builds a linked list of commands from tokens */
t_command	*parse_tokens(t_token *tokens, char **env, int last_exit);

/* 			Executor: executes a command chain (with redirections/pipes) */
void	execute_command(t_command *cmd, char **env, int *last_exit,
		t_token *tokens);

/*			Builtin commands */
int	builtin_pwd(void);
int	builtin_env(char **env);
int	builtin_cd(char **args, char ***env);
int	builtin_exit(char **args);
int	is_parent_builtin(char *cmd);
int	builtin_unset(char **args, char ***env);
int	builtin_export(char **args, char ***env);
int	builtin_echo(char **args, int *last_exit);

/* 			Signal handling */
void	signal_handler(int signo);

/* 			Memory cleanup */
void	free_tokens(t_token *tokens);
void	free_commands(t_command *cmd);

/* 			Utility for variable expansion */
char	*expand_variables(char *str, char **env, int last_exit);
char	*ft_strjoin_char(char *str, char c);

/* 			Tokenizer: splits input into tokens
			(handling quotes and special symbols) */
t_token	*tokenize_input(char *input);

#endif
