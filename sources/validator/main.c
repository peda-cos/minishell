/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:03:12 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/12 21:56:54 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "validator.h"

static int	syntax_error(t_content_params *params)
{
	char	*token_value;

	token_value = get_content_value(params);
	ft_putstr_fd(INVALID_TOKEN_MSG_ERR, STDERR_FILENO);
	ft_putstr_fd(token_value, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	return (TRUE);
}

int	validate_tokens(t_token **tokens, char **env, int *last_exit)
{
	t_token				*token;
	t_content_params	params;

	token = *tokens;
	while (token)
	{
		params.envs = env;
		params.content = token->content;
		params.last_exit_code = last_exit;
		if (is_invalid_token(token, get_content_value(&params)))
			return (syntax_error(&params));
		if (is_pipe(token) && !is_valid_pipe_position(token))
			return (syntax_error(&params));
		if (is_redirection(token) && !is_valid_redirection(token))
			return (syntax_error(&params));
		token = token->next;
	}
	return (FALSE);
}
