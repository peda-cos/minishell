/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gay.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:57:59 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/13 09:59:37 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_colored_prompt(void)
{
	static int	color_index = 0;
	static char	*pastel_colors[6] = {
		"\001\033[38;5;183m\002",
		"\001\033[38;5;217m\002",
		"\001\033[38;5;122m\002",
		"\001\033[38;5;159m\002",
		"\001\033[38;5;229m\002",
		"\001\033[38;5;153m\002"
	};
	static char	prompt[100];

	ft_memset(prompt, 0, sizeof(prompt));
	ft_strlcpy(prompt, pastel_colors[color_index], sizeof(prompt));
	ft_strlcat(prompt, "Minishell $ \001\033[0m\002", sizeof(prompt));
	color_index = (color_index + 1) % 6;
	return (prompt);
}
