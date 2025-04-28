/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:11:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/26 18:40:37 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include <sys/stat.h>

/*
** Path finding and manipulation functions (path_utils.c)
*/
void	free_split(char **split);
char	*find_executable(char *cmd, char **env);

/*
** Heredoc handling functions (heredoc.c)
*/
int		handle_heredoc(char *delim, char **env, int last_exit);
void	preprocess_heredocs(t_command *cmd_list);

/*
** Redirection handling functions (redirection.c)
*/
int		setup_input_redirection(t_command *cmd, char **env, int last_exit);
int		setup_output_redirection(t_process_command_args *param);

/*
** Command execution functions (command_utils.c)
*/
int		is_builtin(char *cmd);
int		execute_builtin(t_process_command_args *arg);
int		execute_external(t_command *cmd, char **env);

/*
** Process management functions (process.c)
*/
int		setup_pipe(t_command *cmd, int pipefd[2]);
void	child_process(t_process_command_args *param);
void	parent_process(t_process_command_args *param);
int		process_command(t_command *cmd, char ***envs, int *last_exit,
			t_token *tokens);

int		setup_file_input(char *input_file);
t_token	*get_fd_redirect_token(char *target_file, t_token *tokens);
int		dup_fd_std(int fd, char *filename, t_process_command_args *arg);
int		set_redirection_to_files(t_process_command_args *arg, int *flags);
int		setup_input_redirection(t_command *cmd, char **env, int last_exit);

#endif
