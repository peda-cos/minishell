/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:11:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/25 08:25:40 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

/*
** Path finding and manipulation functions (path_utils.c)
*/
void	free_split(char **split);
char	*find_executable(char *cmd, char **env);

/*
** Heredoc handling functions (heredoc.c)
*/
int		handle_heredoc(char *delim);

/*
** Redirection handling functions (redirection.c)
*/
int		setup_input_redirection(t_command *cmd);
int		setup_output_redirection(t_command *cmd);

/*
** Command execution functions (command_utils.c)
*/
int		is_builtin(char *cmd);
int		execute_builtin(t_command *cmd, char ***env, int *last_exit);
int		execute_external(t_command *cmd, char **env);

/*
** Process management functions (process.c)
*/
int		setup_pipe(t_command *cmd, int pipefd[2]);
void	child_process(t_command *cmd, int pipefd[2], char **env,
			int *last_exit);
void	parent_process(t_command *cmd, int pipefd[2], pid_t pid,
			int *last_exit);
int		process_command(t_command *cmd, char **env, int *last_exit);

#endif
