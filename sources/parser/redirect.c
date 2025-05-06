/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:37:02 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/05 23:58:12 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * @brief Processes the heredoc delimiter by concatenating its values
 * @param content The token content containing the heredoc delimiter values
 * @param cmd The command structure to set the heredoc delimiter
 * @note Allocates memory for the concatenated
 * delimiter and sets it in the command structure
 */
static void	process_heredoc_delim(t_token_content *content, t_command	*cmd)
{
	char			*temp;
	char			*heredoc_delimiter;
	t_token_content	*tmp_content;

	heredoc_delimiter = ft_strdup("");
	tmp_content = content;
	while (tmp_content)
	{
		temp = heredoc_delimiter;
		heredoc_delimiter = ft_strjoin(temp, tmp_content->value);
		free(temp);
		tmp_content = tmp_content->next;
	}
	cmd->heredoc_delim = ft_strdup(heredoc_delimiter);
	free(heredoc_delimiter);
}

/**
 * @brief Creates an output file for redirection
 * @param filename The name of the file to create
 * @param append Flag indicating whether to append or truncate the file
 * @return 1 on success, 0 on failure
 * @note Opens the file with write-only and create flags
 */
static int	create_output_file(char *filename, int append)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		perror(filename);
		return (0);
	}
	close(fd);
	return (1);
}

/**
 * @brief Sets the output redirect file in the command structure
 * @param value The name of the output file to set
 * @param cmd The command structure to update
 * @param is_append Flag indicating whether to append or truncate file
 * @note Allocates memory for the output file name
 * and sets it in the command structure
 */
void	set_output_redirect_file(char *value,
	t_command	*cmd, int is_append)
{
	int	index;

	index = cmd->amount_output_files;
	if (index < OUTPUT_FILES_MAX_SIZE)
	{
		if (cmd->output_file_list[index])
			free(cmd->output_file_list[index]);
		cmd->output_file_list[index] = value;
		cmd->output_file_list[index + 1] = NULL;
		cmd->amount_output_files++;
		cmd->append = is_append;
	}
}

/**
 * @brief Sets the redirection target in the command structure
 * @param cmd The command structure to update
 * @param token The token containing the redirection type
 * @param content_params The parameters for processing the content
 * @note Processes different redirection types (input, output, append, heredoc)
 */
void	set_redirection_target(t_command *cmd,
	t_token *token, t_content_params *content_params)
{
	char	*filename;

	filename = get_token_content_value(content_params);
	if (!filename)
		return ;
	if (token->type == REDIRECT_IN)
	{
		free(cmd->input_file);
		cmd->input_file = filename;
	}
	else if (token->type == REDIRECT_OUT)
		set_output_redirect_file(filename, cmd, FALSE);
	else if (token->type == APPEND)
		set_output_redirect_file(filename, cmd, TRUE);
	else if (token->type == HEREDOC)
		process_heredoc_delim(content_params->content, cmd);
	else
		free(filename);
}

/**
 * @brief Handles the redirection of input/output files
 * in the command structure
 * @param cmd The command structure to update
 * @param token_ptr Pointer to the current token in the list
 * @param envs The environment variables for expansion
 * @param last_exit Pointer to the last exit status for $? expansion
 * @return 1 on success, 0 on failure
 * @note Updates the command structure
 * with the redirection target and type
 */
int	handle_redirection(t_command *cmd,
	t_token **token_ptr, char **envs, int *last_exit)
{
	t_token				*token;
	int					result;
	t_content_params	params;
	char				*content_value;

	token = *token_ptr;
	*token_ptr = token->next;
	if (!*token_ptr || (*token_ptr)->type != WORD)
		return (ft_putstr_fd(SYNTAX_ERROR_MSG, STDERR_FILENO), 0);
	params.envs = envs;
	params.last_exit_code = last_exit;
	params.content = (*token_ptr)->content;
	if (token->type == REDIRECT_OUT || token->type == APPEND)
	{
		content_value = get_token_content_value(&params);
		if (!content_value)
			return (0);
		result = create_output_file(content_value, token->type == APPEND);
		free(content_value);
		if (!result)
			return (0);
	}
	set_redirection_target(cmd, token, &params);
	*token_ptr = (*token_ptr)->next;
	return (1);
}
