/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:12:39 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/20 22:20:10 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Processes heredoc input content with variable expansion
 * @param stripped_delim The delimiter string without quotes
 * @param expand_vars Flag indicating whether
	* to expand variables in the content
 * @param envs Environment variables array for variable expansion
 * @param last_exit Last command exit status for $? expansion
 * @return Dynamically allocated string
	* containing processed heredoc content
 * @note Reads input line by line until delimiter is encountered
 */
static char	*process_heredoc_content(char *stripped_delim,
	int expand_vars, char **envs, int last_exit)
{
	char	*line;
	char	*buffer;
	char	*processed_line;

	buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	while (TRUE)
	{
		line = readline("📝 ❯ ");
		if (!line || !ft_strcmp(line, stripped_delim))
			return (free_and_return(line, buffer));
		processed_line = process_heredoc_line(line,
				envs, last_exit, expand_vars);
		free(line);
		if (!processed_line)
			return (free_and_return(buffer, NULL));
		buffer = append_to_buffer(buffer, processed_line);
		free(processed_line);
		if (!buffer)
			return (NULL);
	}
}

/**
 * @brief Reads heredoc content from the user
	* until the delimiter is encountered
 * @param delim The delimiter string
 * @return Dynamically allocated string containing heredoc content
 * @note Appends each line with newline
	* character to build complete content
 */
static char	*read_heredoc_content_to_buffer(char *delim)
{
	char	*tmp;
	char	*line;
	char	*buffer;

	buffer = ft_strdup("");
	while (TRUE)
	{
		line = readline("📝 ❯ ");
		if (!line || !ft_strcmp(line, delim))
		{
			free(line);
			break ;
		}
		tmp = buffer;
		buffer = ft_strjoin(buffer, line);
		free(tmp);
		tmp = buffer;
		buffer = ft_strjoin(buffer, "\n");
		free(tmp);
		free(line);
	}
	return (buffer);
}

/**
 * @brief Reads heredoc content and processes it for variable expansion
 * @param delim The delimiter string
 * @param env Environment variables array
 * @param last_exit Last command exit status for $? expansion
 * @return Dynamically allocated string containing processed heredoc content
 * @note Handles quoted delimiters and
	* variable expansion based on delimiter type
 */
static char	*read_heredoc_content(char *delim, char **env, int last_exit)
{
	char	*content;
	int		expand_vars;
	char	*stripped_delim;

	expand_vars = !is_quoted_delimiter(delim);
	stripped_delim = get_stripped_delim(expand_vars, delim);
	if (!stripped_delim)
		return (NULL);
	content = process_heredoc_content(stripped_delim,
			expand_vars, env, last_exit);
	free(stripped_delim);
	if (!content)
		return (NULL);
	return (content);
}

/**
 * @brief Handles heredoc input and returns a file descriptor for reading
 * @param delim The delimiter string
 * @param env Environment variables array
 * @param last_exit Last command exit status for $? expansion
 * @return File descriptor for reading heredoc content, or -1 on error
 * @note Creates a pipe and writes the processed heredoc content to it
 */
int	handle_heredoc(char *delim, char **env, int last_exit)
{
	int		pipefd[2];
	char	*buffer;

	if (!delim || pipe(pipefd) < 0)
		return (-1);
	signal(SIGINT, SIG_DFL);
	buffer = read_heredoc_content(delim, env, last_exit);
	if (!buffer)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	write(pipefd[1], buffer, ft_strlen(buffer));
	close(pipefd[1]);
	free(buffer);
	return (pipefd[0]);
}

/**
 * @brief Preprocesses heredocs in the command list
 * @param cmd_list Pointer to the command list
 * @return void
 * @note Creates pipes for each command with heredoc delimiter
 *       Stores read file descriptor in command structure
 */
void	preprocess_heredocs(t_command *cmd_list, char **envs, int *last_exit)
{
	t_command	*cmd;
	char		*buffer;
	int			pipefd[2];

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->heredoc_delim)
		{
			if (pipe(pipefd) < 0)
				return (perror("pipe"));
			buffer = read_heredoc_content_to_buffer(cmd->heredoc_delim);
			buffer = process_expanded_heredoc(cmd, buffer, envs, last_exit);
			if (buffer)
			{
				write(pipefd[1], buffer, ft_strlen(buffer));
				free(buffer);
			}
			close(pipefd[1]);
			cmd->heredoc_fd = pipefd[0];
		}
		else
			cmd->heredoc_fd = -1;
		cmd = cmd->next;
	}
}
