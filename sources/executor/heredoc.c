/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:12:39 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/08 06:37:31 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Processes heredoc input content with variable expansion
 * @param ctx The heredoc processing context
 * @return Dynamically allocated string containing processed heredoc content
 * @note Reads input line by line until delimiter is encountered
 */
static char	*process_heredoc_content(t_heredoc_ctx *ctx)
{
	char	*line;
	char	*buffer;

	buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	while (TRUE)
	{
		line = readline("📝 ❯ ");
		if (!line)
		{
			display_heredoc_eof_warning(ctx->stripped_delim);
			return (free_and_return(line, buffer));
		}
		buffer = process_single_heredoc_line(ctx, line, buffer);
		if (!buffer)
			return (NULL);
	}
}

/**
 * @brief Reads heredoc content and processes it for variable expansion
 * @param delim The delimiter string
 * @param env Environment variables array
 * @param last_exit Last command exit status for $? expansion
 * @return Dynamically allocated string containing processed heredoc content

	* @note Handles quoted delimiters and variable expansion based on delimiter type
 */
static char	*read_heredoc_content(char *delim, char **env, int last_exit)
{
	char			*content;
	char			*stripped_delim;
	t_heredoc_ctx	ctx;

	ctx.expand_vars = !is_quoted_delimiter(delim);
	stripped_delim = get_stripped_delim(ctx.expand_vars, delim);
	if (!stripped_delim)
		return (NULL);
	ctx.stripped_delim = stripped_delim;
	ctx.envs = env;
	ctx.last_exit = &last_exit;
	content = process_heredoc_content(&ctx);
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
 * @param envs Environment variables array
 * @param last_exit Last command exit status
 * @return void
 * @note Creates pipes for each command with heredoc delimiter
 *       Stores read file descriptor in command structure
 */
void	preprocess_heredocs(t_command *cmd_list, char **envs, int *last_exit)
{
	t_command		*cmd;
	t_heredoc_ctx	ctx;

	cmd = cmd_list;
	ctx.envs = envs;
	ctx.last_exit = last_exit;
	while (cmd)
	{
		if (cmd->heredoc_delim)
		{
			if (process_single_command_heredoc(cmd, &ctx) < 0)
				return ;
		}
		else
			cmd->heredoc_fd = -1;
		cmd = cmd->next;
	}
}
