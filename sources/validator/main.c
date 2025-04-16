/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:03:12 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/16 20:01:17 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "validator.h"

static int	print_syntax_error(char *token_value)
{
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
		if (is_pipe(token) && !is_valid_pipe_position(token))
			return (print_syntax_error(token->content->value));
		if (is_redirection(token->type) && !is_valid_redirection(token))
			return (print_syntax_error(token->content->value));
		token = token->next;
	}
	return (FALSE);
}
