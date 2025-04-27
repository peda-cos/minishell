/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 22:31:37 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/26 23:56:26 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

typedef struct s_tokenizer_ctx
{
	char	*str;
	char	*word;
	char	delimiter;
	int		in_quotes;
	int		word_index;
	int		*str_index;
}	t_token_word_param;

t_token			*get_last_token(t_token **tokens);
void			add_token(t_token **tokens, t_token *new);
t_token			*new_token(t_token_content *content, t_token_type type);
t_token_content	*new_token_content(char *value,
					int in_quotes, int in_single_quotes);

int				is_metachar(char c);
int				is_quote_delimiter(char *str, int *index, char delimiter);

void			add_token_word(char *str, int *index, t_token **tokens);
void			add_token_redirect(char *input, int *index, t_token **tokens);

#endif
