/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 22:31:37 by jlacerda          #+#    #+#             */
/*   Updated: 2025/03/27 22:35:53 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

int		is_metachar(char c);
void	add_token(t_token **tokens, t_token *new);
t_token	*new_token(char *value, t_token_type type);
int		is_quote_delimiter(char *str, int *index, char delimiter);

#endif
