/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:20:01 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/01 22:03:51 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"

typedef struct s_expansion_ctx
{
	char	**env;
	int		last_exit;
}			t_expansion_ctx;

/* Variable expansion functions */
char		*get_env_value(char *key, char **env);
char		*handle_exit_status(int last_exit);
char		*handle_variable(char *str, int *i, char **env, int last_exit);
char		*process_var_expansion(char *result, char *str, int *i,
				t_expansion_ctx *ctx);
char		*expand_variables(char *str, char **env, int last_exit);

#endif
