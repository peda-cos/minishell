/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:03:12 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/14 11:55:25 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "validator.h"

int	is_redirection(t_token *token)
{
	return (token->type == REDIRECT_IN || token->type == REDIRECT_OUT
		|| token->type == HEREDOC || token->type == APPEND);
}

int	is_pipe(t_token *token)
{
	return (token->type == PIPE);
}

int	is_invalid_token(t_token *token)
{
	if (token->type == WORD && (!token->value || strlen(token->value) == 0))
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
