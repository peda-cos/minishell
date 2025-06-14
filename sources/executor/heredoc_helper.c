/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 06:32:27 by peda-cos          #+#    #+#             */
/*   Updated: 2025/06/14 16:06:58 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Displays EOF warning message for heredoc
 * @param delimiter The delimiter that was expected
 * @note Prints warning when heredoc is terminated by EOF instead of delimiter
 */
void	display_heredoc_eof_warning(char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document delimited by ",
		STDERR_FILENO);
	ft_putstr_fd("end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(delimiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

/**
 * @brief Processes a single line in heredoc buffer
 * @param ctx The heredoc context structure
 * @param line The input line
 * @param buffer The current buffer
 * @return Updated buffer with processed line
 */
char	*process_buffer_line(t_heredoc_ctx *ctx, char *line, char *buffer)
{
	char	*tmp;

	line = process_expanded_heredoc(ctx->cmd, line, ctx->envs, ctx->last_exit);
	tmp = buffer;
	buffer = ft_strjoin(buffer, line);
	free(tmp);
	tmp = buffer;
	buffer = ft_strjoin(buffer, "\n");
	free(tmp);
	free(line);
	return (buffer);
}

/**
 * @brief Processes a single line from heredoc input
 * @param ctx The heredoc processing context
 * @param line The input line to process
 * @param buffer The current buffer to append to
 * @return Updated buffer with processed line, or NULL on error
 */
char	*process_single_heredoc_line(t_heredoc_ctx *ctx, char *line,
		char *buffer)
{
	char	*processed_line;

	if (!ft_strcmp(line, ctx->stripped_delim))
		return (free_and_return(line, buffer));
	processed_line = process_heredoc_line(line, ctx->envs, *ctx->last_exit,
			ctx->expand_vars);
	free(line);
	if (!processed_line)
		return (free_and_return(buffer, NULL));
	buffer = append_to_buffer(buffer, processed_line);
	free(processed_line);
	if (!buffer)
		return (NULL);
	return (buffer);
}

/**
 * @brief Reads heredoc content from the user until delimiter is encountered
 * @param ctx The heredoc processing context
 * @return Dynamically allocated string containing heredoc content
 * @note Appends each line with newline character to build complete content
 */
char	*read_heredoc_content_to_buffer(t_heredoc_ctx *ctx)
{
	char	*line;
	char	*buffer;

	buffer = ft_strdup("");
	while (TRUE)
	{
		line = readline("📝 ❯ ");
		if (!line)
		{
			display_heredoc_eof_warning(ctx->cmd->heredoc_delim);
			free(line);
			break ;
		}
		if (!ft_strcmp(line, ctx->cmd->heredoc_delim))
		{
			free(line);
			break ;
		}
		buffer = process_buffer_line(ctx, line, buffer);
	}
	return (buffer);
}

/**
 * @brief Processes heredoc for a single command
 * @param cmd The command to process
 * @param ctx The heredoc processing context
 * @return 0 on success, -1 on error
 * @note Creates a pipe and processes the heredoc content for the command
 */
int	process_single_command_heredoc(t_command *cmd, t_heredoc_ctx *ctx)
{
	char *buffer = NULL;
	int pipefd[2];
	pid_t pid;
	int status;

	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (-1);
	}
	ctx->cmd = cmd;
	// Criar um processo filho para lidar com o heredoc
	pid = fork();

	if (pid == 0) // Processo filho
	{
		// No processo filho, podemos usar SIG_DFL com segurança
		signal(SIGINT, SIG_DFL);
		close(pipefd[0]);
		
		buffer = read_heredoc_content_to_buffer(ctx);
		if (buffer)
		{
			write(pipefd[1], buffer, ft_strlen(buffer));
			free(buffer);
		}
		close(pipefd[1]);
		exit(0); // Sair do processo filho
	}
	else // Processo pai
	{
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		
		// Se o filho foi terminado por SIGINT
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			*ctx->last_exit = 130; // Código de saída para SIGINT
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		
		cmd->heredoc_fd = pipefd[0];
		return (0);
	}
	return (0);
}
