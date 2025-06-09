/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:03:12 by jlacerda          #+#    #+#             */
/*   Updated: 2025/06/08 21:15:36 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "validator.h"

/**
 * @brief Prints a syntax error message for an invalid token
 * @param token_value The value of the invalid token
 * @return 1 if the error is printed, 0 otherwise
 * @note Prints the error message to standard error output
 */
static int	print_syntax_error(char *token_value)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(INVALID_TOKEN_MSG_ERR, STDERR_FILENO);
	ft_putstr_fd(token_value, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	return (TRUE);
}

/**
 * @brief Validates the tokens in the linked list
 * @param tokens The head of the token list
 * @return 1 if there is a syntax error, 0 otherwise
 * @note Iterates through each token and checks for 
	* invalid pipe and redirection positions
 */
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
		if (token->has_unclosed_quotes)
			return (TRUE);
		token = token->next;
	}
	return (FALSE);
}
