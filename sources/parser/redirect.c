/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:37:02 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/03 20:14:55 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
	free(filename);
}

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
