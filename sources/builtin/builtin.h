/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:15:36 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/29 20:10:43 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

/* Environment management */
char	**copy_env(char **envp);
int		find_env_index(char *key, char **env);

/* Builtin commands */
int		builtin_echo(char **args, int *last_exit);
int		builtin_cd(char **args);
int		builtin_pwd(void);
int		builtin_export(char **args, char ***env);
int		builtin_unset(char **args, char ***env);
int		builtin_env(char **env);
int		builtin_exit(char **args);
int		is_parent_builtin(char *cmd);

#endif