/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:15:26 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/08 23:09:40 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H
# define TRUE 1
# define FALSE 0

# define NULL_CHR '\0'
# define PIPE_CHR '|'
# define BACKSLASH_CHR '\\'
# define DOT_AND_COMA_CHR ';'
# define REDIRECT_IN_CHR '<'
# define REDIRECT_OUT_CHR '>'
# define SINGLE_QUOTE_CHR '\''
# define DOLLAR_CHR '$'
# define BRACKET_OPEN_CHR '{'
# define BRACKET_CLOSE_CHR '}'
# define UNDERSCORE_CHR '_'
# define EQUAL_CHR '='
# define DASH_CHR '-'
# define SPACE_CHR ' '
# define EMPTY_STR ""
# define DASH_STR "-"
# define EQUAL_STR "="
# define PIPE_STR "|"
# define DOUBLE_QUOTE_CHR '"'
# define APPEND_STR ">>"
# define HEREDOC_STR "<<"
# define REDIRECT_IN_STR "<"
# define REDIRECT_OUT_STR ">"
# define SINGLE_QUOTE_STR "'"
# define DOUBLE_QUOTE_STR "\""
# define FD_LIST_SIZE 42

/* Prompt ---------------- */
# define MAX_PROMPT_LENGTH 1024
# define ANSI_ESC_START "\001\033["
# define ANSI_ESC_END "\002"
# define ANSI_RESET "\001\033[0m\002"
# define COLOR_PASTEL_WHITE "\001\033[38;5;255m\002"
# define COLOR_PASTEL_ORANGE "\001\033[38;5;214m\002"
# define COLOR_PASTEL_PURPLE "\001\033[38;5;183m\002"
# define COLOR_PASTEL_PINK "\001\033[38;5;217m\002"
# define COLOR_PASTEL_GREEN "\001\033[38;5;122m\002"
# define COLOR_PASTEL_YELLOW "\001\033[38;5;229m\002"
# define COLOR_PASTEL_BLUE "\001\033[38;5;153m\002"
# define COLOR_RESET "\001\033[0m\002"
# define PROMPT_TEXT "🧙 Minishell ❯ "
/* ----------------------- */
#endif
