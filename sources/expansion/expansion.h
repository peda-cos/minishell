/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:50:33 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/31 21:50:35 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:20:01 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/31 14:21:15 by peda-cos         ###   ########.fr       */
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
