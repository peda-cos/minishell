/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:15:36 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/08 07:22:24 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

# define EXPORT_OPTIONS_CHARS "afnp"
# define UNSET_OPTIONS_CHARS "fnv"

/* Environment management */
char	**copy_env(char **envp);
int		find_env_index(char *key, char **env);

/* Builtin commands */
int		is_parent_builtin(char *cmd);
int		builtin_echo(char **args, int *last_exit);
int		builtin_cd(char **args, char ***env);
int		builtin_pwd(void);
int		builtin_export(char **args, char ***env);
int		builtin_unset(char **args, char ***env);
int		builtin_env(char **args, char **env);
int		builtin_exit(t_process_command_args *param);

/* Utility functions */
int		is_valid_key(char *key);
int		print_invalid_arg(char *arg);
char	*get_env_value(char *key, char **envs);
void	print_invalid_option(char *builtin, char *arg, char *arg_options);
int		display_bash_export(char **env);
void	sort_env_alphabetically(char **sorted_env, int count);

/* CD Utility functions */
void	update_pwd_env(char *old_pwd, char ***env);
int		change_to_home(char ***env, char *old_pwd);
int		change_to_previous(char ***env, char *old_pwd);
int		change_directory(char *dir, char ***env, char *old_pwd);
void	update_env_variable(char *key, char *value, char ***env);

#endif
