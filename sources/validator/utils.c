/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:03:12 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/16 19:51:37 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "validator.h"

int	is_pipe(t_token *token)
{
	return (token->type == PIPE);
}

int	is_valid_pipe_position(t_token *token)
{
	if (!token->prev || !token->next)
		return (FALSE);
	if (token->prev->type != WORD)
		return (FALSE);
	if (token->next->type != WORD && !(is_redirection(token->next->type)
			&& token->next->next && token->next->next->type == WORD))
		return (FALSE);
	return (TRUE);
}

int	is_valid_redirection(t_token *token)
{
	if (!token->next || token->next->type != WORD)
		return (FALSE);
	return (TRUE);
}
