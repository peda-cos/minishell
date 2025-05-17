/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 22:31:37 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/17 17:25:27 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

typedef struct s_tokenizer
{
	char	*str;
	char	*word;
	char	delimiter;
	int		in_quotes;
	int		word_index;
	int		*str_index;
}	t_tokenizer;

int				is_metachar(char c);
t_token			*get_last_token(t_token **tokens);
void			add_token(t_token **tokens, t_token *new);
char			*get_word_in_brackets(char *str, int *index);
int				init_tkz_word_params(t_tokenizer *tkz, char *str, int *index);
t_token			*new_token(t_token_content *content, t_token_type type);
void			add_token_word(char *str, int *index, t_token **tokens);
int				is_quote_delimiter(char *str, int *index, char delimiter);
char			*get_word_in_quotes(char *str, int *index, char delimiter);
void			add_token_redirect(char *input, int *index, t_token **tokens);

t_token_content	*get_new_token_content(t_tokenizer *tkz);
t_token_content	*new_token_content(
					char *value, int in_quotes, int in_single_quotes);
t_token_content	*append_token_content(
					t_token_content *content, t_tokenizer *tkz);
t_token_content	*add_only_content_word(
					t_token_content *content, t_tokenizer *tkz);
t_token_content	*process_word_in_quotes(
					t_token_content *content, t_tokenizer *tkz);
t_token_content	*process_word_in_brackets(
					t_token_content *content, t_tokenizer *tkz);

#endif
