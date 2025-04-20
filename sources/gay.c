/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gay.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:57:59 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/20 19:23:33 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	reset_prompt(char *prompt, int color_index, char **pastel_colors)
{
	ft_memset(prompt, 0, MAX_PROMPT_LENGTH);
	ft_strlcpy(prompt, pastel_colors[color_index], MAX_PROMPT_LENGTH);
}

static void	append_username_to_prompt(char *prompt,
	int color_index, char **pastel_colors)
{
	char	*username;

	username = getenv("USER");
	(void)color_index;
	(void)pastel_colors;
	if (username)
	{
		ft_strlcat(prompt, COLOR_PASTEL_ORANGE, MAX_PROMPT_LENGTH);
		ft_strlcat(prompt, "( ͡° ͜ʖ ͡°) ", MAX_PROMPT_LENGTH);
		ft_strlcat(prompt, username, MAX_PROMPT_LENGTH);
		ft_strlcat(prompt, " ", MAX_PROMPT_LENGTH);
		ft_strlcat(prompt, pastel_colors[color_index], MAX_PROMPT_LENGTH);
	}
}

static void	append_directory_to_prompt(char *prompt,
	char *current_dir, int color_index, char **pastel_colors)
{
	char	*home_dir;

	home_dir = getenv("HOME");
	if (home_dir && ft_strncmp(current_dir, home_dir, strlen(home_dir)) == 0)
	{
		ft_strlcat(prompt, pastel_colors[color_index], MAX_PROMPT_LENGTH);
		ft_strlcat(prompt, "~", MAX_PROMPT_LENGTH);
		ft_strlcat(prompt, current_dir + strlen(home_dir), MAX_PROMPT_LENGTH);
	}
	else
		ft_strlcat(prompt, current_dir, MAX_PROMPT_LENGTH);
	ft_strlcat(prompt, COLOR_PASTEL_WHITE, MAX_PROMPT_LENGTH);
	ft_strlcat(prompt, " ❯ ", MAX_PROMPT_LENGTH);
}

static void	config_prompt_text(char *prompt,
	int color_index, char **pastel_colors)
{
	char		*current_dir;

	current_dir = getcwd(NULL, 0);
	append_username_to_prompt(prompt, color_index, pastel_colors);
	if (current_dir)
	{
		ft_strlcat(prompt, "▄︻デ══━一 ", MAX_PROMPT_LENGTH);
		append_directory_to_prompt(prompt,
			current_dir, color_index, pastel_colors);
		free(current_dir);
	}
	else
		ft_strlcat(prompt, PROMPT_TEXT, MAX_PROMPT_LENGTH);
}

char	*get_colored_prompt(void)
{
	static int	color_index = 0;
	static char	*pastel_colors[5] = {
		COLOR_PASTEL_PURPLE,
		COLOR_PASTEL_PINK,
		COLOR_PASTEL_GREEN,
		COLOR_PASTEL_YELLOW,
		COLOR_PASTEL_BLUE
	};
	static char	prompt[MAX_PROMPT_LENGTH];

	reset_prompt(prompt, color_index, pastel_colors);
	config_prompt_text(prompt, color_index, pastel_colors);
	ft_strlcat(prompt, COLOR_RESET, MAX_PROMPT_LENGTH);
	color_index = (color_index + 1) % 5;
	return (prompt);
}
