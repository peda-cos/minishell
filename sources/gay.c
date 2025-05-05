/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gay.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:57:59 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 18:42:15 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Clears the prompt buffer and initializes it with the current color
 * @param prompt The buffer to store the formatted prompt
 * @param color_index Index of the current pastel color to use
 * @param pastel_colors Array of ANSI color code strings
 * @return void
 * @note Zeroes out the entire prompt buffer before setting the initial color
 */
static void	reset_prompt(char *prompt, int color_index, char **pastel_colors)
{
	ft_memset(prompt, 0, MAX_PROMPT_LENGTH);
	ft_strlcpy(prompt, pastel_colors[color_index], MAX_PROMPT_LENGTH);
}

/**
 * @brief Adds the username to the prompt with decorative elements
 * @param prompt The buffer to append the username information to
 * @param color_index Index of the current pastel color
 * @param pastel_colors Array of ANSI color code strings
 * @return void
 * @note Appends the username, a wizard emoji, and a fire emoji to the prompt
 */
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
		ft_strlcat(prompt, "🧙 ", MAX_PROMPT_LENGTH);
		ft_strlcat(prompt, username, MAX_PROMPT_LENGTH);
		ft_strlcat(prompt, " 🔥 ", MAX_PROMPT_LENGTH);
		ft_strlcat(prompt, pastel_colors[color_index], MAX_PROMPT_LENGTH);
	}
}

/**
 * @brief Appends the current directory to the prompt
 * @param prompt The buffer to append the directory information to
 * @param current_dir The current working directory
 * @param color_index Index of the current pastel color
 * @param pastel_colors Array of ANSI color code strings
 * @return void
 * @note Replaces the home directory with a tilde (~) if applicable
 */
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

/**
 * @brief Configures the prompt text with username and current directory
 * @param prompt The buffer to store the formatted prompt
 * @param color_index Index of the current pastel color
 * @param pastel_colors Array of ANSI color code strings
 * @return void
 * @note Appends the username and current directory to the prompt buffer
 */
static void	config_prompt_text(char *prompt,
	int color_index, char **pastel_colors)
{
	char		*current_dir;

	current_dir = getcwd(NULL, 0);
	append_username_to_prompt(prompt, color_index, pastel_colors);
	if (current_dir)
	{
		append_directory_to_prompt(prompt,
			current_dir, color_index, pastel_colors);
		free(current_dir);
	}
	else
		ft_strlcat(prompt, PROMPT_TEXT, MAX_PROMPT_LENGTH);
}

/**
 * @brief Generates a colored prompt string with rotating pastel colors
 * @return A pointer to the formatted prompt string
 * @note The prompt includes the username, current directory, and emojis
 *       The color changes each time the function is called
 */
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
