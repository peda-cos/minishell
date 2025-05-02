/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:20:01 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/28 21:07:16 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"

char	*get_env_value(char *key, char **envs);
char	*get_underscore_arg_value(char **envs);
void	set_underscore_arg_value(t_command *cmd, char ***envs);
char	*extract_var_name_in_brackets_entered(char *str, int *index);
char	*get_special_variable(char *str,
			int *index, char **envs, int last_exit);

#endif
