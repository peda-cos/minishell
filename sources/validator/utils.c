/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:03:12 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/04 19:12:24 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "validator.h"

/**
 * @brief Checks if the token is a pipe
 * @param token The token to check
 * @return 1 if it is a pipe, 0 otherwise
 * @note Compares the token type with the PIPE constant
 */
int	is_pipe(t_token *token)
{
	return (token->type == PIPE);
}

/**
 * @brief Checks if a pipe token is in a valid position
 * @param token The pipe token to check
 * @return 1 if the pipe position is valid, 0 otherwise
 * @note Validates that a pipe has valid tokens before and after it
 */
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

/**
 * @brief Checks if a redirection token is in a valid position
 * @param token The redirection token to check
 * @return 1 if the redirection position is valid, 0 otherwise
 * @note Validates that a redirection is followed by a word token
 */
int	is_valid_redirection(t_token *token)
{
	if (!token->next || token->next->type != WORD)
		return (FALSE);
	return (TRUE);
}
