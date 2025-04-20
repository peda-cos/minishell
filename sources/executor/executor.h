/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:11:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/20 00:55:05 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include <sys/stat.h>

typedef struct s_process_command_args
{
	int			pid;
	char		**env;
	t_command	*cmd;
	int			pipefd[2];
	int			*last_exit;
	t_token		*tokens;
}				t_process_command_args;

/*
** Path finding and manipulation functions (path_utils.c)
*/
void			free_split(char **split);
char			*find_executable(char *cmd, char **env);

/*
** Heredoc handling functions (heredoc.c)
*/
int				handle_heredoc(char *delim, char **env, int last_exit);

/*
** Redirection handling functions (redirection.c)
*/
int				setup_input_redirection(t_command *cmd, char **env,
					int last_exit);
int				setup_output_redirection(t_command *cmd);

/*
** Command execution functions (command_utils.c)
*/
int				is_builtin(char *cmd);
int				execute_builtin(t_command *cmd, char ***env, int *last_exit);
int				execute_external(t_command *cmd, char **env);

/*
** Process management functions (process.c)
*/
int				setup_pipe(t_command *cmd, int pipefd[2]);
void			child_process(t_process_command_args *param);
void			parent_process(t_process_command_args *param);
int				process_command(t_command *cmd, char **env, int *last_exit,
					t_token *tokens);

#endif
