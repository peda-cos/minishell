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
	* @brief Checks if the token type is a redirection
	* @param type The token type to check
	* @return 1 if it is a redirection, 0 otherwise
	* @note Redirections include input and output redirections
	*/
int	is_pipe(t_token *token)
{
	return (token->type == PIPE);
}

/**
	* @brief Checks if the token type is a redirection
	* @param type The token type to check
	* @return 1 if it is a redirection, 0 otherwise
	* @note Redirections include input and output redirections
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
	* @brief Checks if the token type is a redirection
	* @param type The token type to check
	* @return 1 if it is a redirection, 0 otherwise
	* @note Redirections include input and output redirections
	*/
int	is_valid_redirection(t_token *token)
{
	if (!token->next || token->next->type != WORD)
		return (FALSE);
	return (TRUE);
}
