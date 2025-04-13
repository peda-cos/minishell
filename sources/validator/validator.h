/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:20:01 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/12 21:52:24 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATOR_H
# define VALIDATOR_H

# include "minishell.h"

# define INVALID_TOKEN_MSG_ERR	"Erro de sintaxe próximo ao token inesperado `"

int	is_pipe(t_token *token);
int	is_redirection(t_token *token);
int	is_valid_redirection(t_token *token);
int	is_valid_pipe_position(t_token *token);
int	is_invalid_token(t_token *token, char *value);

#endif
