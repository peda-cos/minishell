/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 22:31:37 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/12 18:56:41 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

int		is_metachar(char c);
t_token	*get_last_token(t_token **tokens);
void	add_token(t_token **tokens, t_token *new);
int		is_quote_delimiter(char *str, int *index, char delimiter);
t_token	*new_token(t_token_content *content, t_token_type type);

#endif
