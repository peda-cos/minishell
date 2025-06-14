/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:11:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/14 14:53:45 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include <sys/stat.h>

/*
** Structure for heredoc processing context to reduce function arguments
*/
typedef struct s_heredoc_ctx
{
	char		*stripped_delim;
	int			expand_vars;
	char		**envs;
	int			*last_exit;
	t_command	*cmd;
}				t_heredoc_ctx;

/*
** Structure for pipe information to reduce function arguments
*/
typedef struct s_pipe_info
{
	int			*fildes;
	t_command	*cmd_left;
	t_command	*cmd_right;
	int			left_right;
}				t_pipe_info;

/*
** Structure for execution context to reduce function arguments
*/
typedef struct s_execution_context
{
	int			*last_exit;
	t_token		*tokens;
}				t_execution_context;

/*
** Path finding and manipulation functions (path_utils.c)
*/
void			free_split(char **split);
char			*find_executable(char *cmd, char **env);

/*
** Heredoc handling functions (heredoc.c)
*/
char			*free_and_return(char *str, char *result);
int				handle_heredoc(char *delim, char **env, int last_exit);
char			*append_to_buffer(char *buffer, char *line);
int				is_quoted_delimiter(char *delim);
char			*get_stripped_delim(int expand_vars, char *delim);
void			preprocess_heredocs(t_command *cmd_list, char **envs,
					int *last_exit);
char			*process_heredoc_line(char *line, char **env, int last_exit,
					int expand_vars);
char			*process_expanded_heredoc(t_command *cmd, char *content,
					char **envs, int *last_exit);

/*
** Heredoc helper functions (heredoc_helper.c)
*/
void			display_heredoc_eof_warning(char *delimiter);
char			*process_buffer_line(t_heredoc_ctx *ctx, char *line,
					char *buffer);
char			*process_single_heredoc_line(t_heredoc_ctx *ctx, char *line,
					char *buffer);
int				process_single_command_heredoc(t_command *cmd,
					t_heredoc_ctx *ctx);
char			*read_heredoc_content_to_buffer(t_heredoc_ctx *ctx);

/*
** Redirection handling functions (redirection.c)
*/
int				setup_input_redirection(t_command *cmd, char **env,
					int last_exit);
int				setup_output_redirection(t_process_command_args *param);

/*
** Command execution functions (command_utils.c)
*/
int				is_builtin(char *cmd);
int				execute_builtin(t_process_command_args *arg);
int				execute_external(t_command *cmd, char **env,
					int is_redirect_out);

/*
** Process management functions (process.c)
*/
int				has_redirect_out(t_token *tokens);
int				setup_pipe(t_command *cmd, int pipefd[2]);
int				setup_child_io(t_process_command_args *arg);
void			child_process(t_process_command_args *param);
void			parent_process(t_process_command_args *param);
int				process_command(t_command *cmd, t_process_command_args *args);

/*
** Pipe handling functions (pipe_handler.c)
*/
int				handle_pipe(t_command *cmd_left, t_command *cmd_right,
					t_process_command_args *args, int is_last_pipe);

/*
** Execution setup functions (execution_setup.c)
*/
int				setup_command_execution(t_command *cmd, char **env,
					int *last_exit);
int				has_pipeline(t_command *cmd);
void			cleanup_command_execution(int pipeline_exists,
					int stdin_backup);

/*
** Command chain functions (command_chain.c)
*/
int				process_command_chain(t_command *cmd_head, char ***env,
					int *last_exit, t_token *tokens);

/*
** Pipe fork functions (pipe_fork.c)
*/
void			fork_process_pipe(t_pipe_info *pipe_info,
					t_process_command_args *args);

/*
** Pipe chain functions (pipe_chain.c)
*/
void			fork_process_pipe_chain(int *fildes, t_command *cmd_start,
					t_process_command_args *args);

/*
** Pipe wait functions (pipe_wait.c)
*/
void			wait_restore_fds_pipe(int *fildes, int *forks,
					int *last_exit, int is_last_pipe);

int				setup_file_input(char *input_file);
t_token			*get_fd_redirect_token(char *target_file, t_token *tokens);
int				dup_fd_std(int fd, char *filename, t_process_command_args *arg);
int				set_redirection_to_files(t_process_command_args *arg,
					int *flags);
int				setup_input_redirection(t_command *cmd, char **env,
					int last_exit);

#endif
