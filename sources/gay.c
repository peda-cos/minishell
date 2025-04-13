/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gay.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:57:59 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/13 12:22:22 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_colored_prompt(void)
{
	static int	color_index = 0;
    static char *pastel_colors[5] = {
        COLOR_PASTEL_PURPLE,
        COLOR_PASTEL_PINK,
        COLOR_PASTEL_GREEN,
        COLOR_PASTEL_YELLOW,
        COLOR_PASTEL_BLUE
    };
	static char	prompt[MAX_PROMPT_LENGTH];

	ft_memset(prompt, 0, sizeof(prompt));
	ft_strlcpy(prompt, pastel_colors[color_index], sizeof(prompt));
	ft_strlcat(prompt, PROMPT_TEXT COLOR_RESET, sizeof(prompt));
	color_index = (color_index + 1) % 5;
	return (prompt);
}
