/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:15:26 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/13 12:19:45 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H
# define TRUE 1
# define FALSE 0

/* Tokenizer ------------ */
# define PIPE_CHR '|'
# define BACKSLASH_CHR '\\'
# define DOT_AND_COMA_CHR ';'
# define REDIRECT_IN_CHR '<'
# define REDIRECT_OUT_CHR '>'
# define SINGLE_QUOTE_CHR '\''
# define DOUBLE_QUOTE_CHR '"'
# define PIPE_STR "|"
# define APPEND_STR ">>"
# define HEREDOC_STR "<<"
# define REDIRECT_IN_STR "<"
# define REDIRECT_OUT_STR ">"
/* ----------------------- */

/* Prompt ---------------- */
# define MAX_PROMPT_LENGTH 1024
# define ANSI_ESC_START "\001\033["
# define ANSI_ESC_END "\002"
# define ANSI_RESET "\001\033[0m\002"
# define COLOR_PASTEL_PURPLE "\001\033[38;5;183m\002"
# define COLOR_PASTEL_PINK "\001\033[38;5;217m\002"
# define COLOR_PASTEL_GREEN "\001\033[38;5;122m\002"
# define COLOR_PASTEL_YELLOW "\001\033[38;5;229m\002"
# define COLOR_PASTEL_BLUE "\001\033[38;5;153m\002"
# define COLOR_RESET "\001\033[0m\002"
# define PROMPT_TEXT "Minishell $ "
/* ----------------------- */
#endif
