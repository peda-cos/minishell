/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:03:12 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/05 19:42:26 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "validator.h"

static int	syntax_error(t_token *token)
{
	char	*token_value;

	token_value = token->value;
	ft_putstr_fd(INVALID_TOKEN_MSG_ERR, STDERR_FILENO);
	ft_putstr_fd(token_value, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	return (TRUE);
}

int	validate_tokens(t_token **tokens)
{
	t_token	*token;

	token = *tokens;
	while (token)
	{
		if (is_invalid_token(token))
			return (syntax_error(token));
		if (is_pipe(token) && !is_valid_pipe_position(token))
			return (syntax_error(token));
		if (is_redirection(token) && !is_valid_redirection(token))
			return (syntax_error(token));
		token = token->next;
	}
	return (FALSE);
}
