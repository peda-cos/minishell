/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:03:12 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/14 22:39:19 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "validator.h"

int	is_pipe(t_token *token)
{
	return (token->type == PIPE);
}

int	is_invalid_token(t_token *token)
{
	int	is_empty_or_null;

	is_empty_or_null = !token->content->value
		|| strlen(token->content->value) == 0;
	if (token->type == WORD && is_empty_or_null)
		return (TRUE);
	if (is_pipe(token) && (!token->prev || !token->next))
		return (TRUE);
	return (FALSE);
}

int	is_valid_pipe_position(t_token *token)
{
	if (!token->prev || !token->next)
		return (FALSE);
	if (token->prev->type != WORD)
		return (FALSE);
	if (token->next->type != WORD && !(is_redirection(token->next)
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
