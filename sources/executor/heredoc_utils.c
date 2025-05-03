/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:12:34 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/02 20:05:13 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*append_to_buffer(char *buffer, char *line)
{
	char	*temp;
	char	*result;

	temp = buffer;
	buffer = ft_strjoin(buffer, line);
	free(temp);
	if (!buffer)
		return (NULL);
	temp = buffer;
	result = ft_strjoin(buffer, "\n");
	free(temp);
	return (result);
}

int	is_quoted_delimiter(char *delim)
{
	int	len;

	len = ft_strlen(delim);
	return (len >= 2 && delim[0] == '\'' && delim[len - 1] == '\'');
}

char	*process_heredoc_line(char *line, char **env, int last_exit,
		int expand_vars)
{
	char			*expanded;
	int				has_error;
	t_expansion_ctx	expansion_cxt;

	has_error = FALSE;
	if (!expand_vars)
		return (ft_strdup(line));
	expansion_cxt.envs = env;
	expansion_cxt.last_exit = &last_exit;
	expansion_cxt.has_error_flag_control = &has_error;
	expanded = expand_variable(line, &expansion_cxt);
	return (expanded);
}

char	*get_stripped_delim(int expand_vars, char *delim)
{
	char	*stripped_delim;

	if (!expand_vars && delim[0] == '\'')
	{
		stripped_delim = ft_substr(delim, 1, ft_strlen(delim) - 2);
		if (!stripped_delim)
			return (NULL);
	}
	else
		stripped_delim = ft_strdup(delim);
	if (!stripped_delim)
		return (NULL);
	return (stripped_delim);
}
