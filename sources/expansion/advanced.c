/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:50:10 by jlacerda          #+#    #+#             */
/*   Updated: 2025/04/24 23:30:31 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static int	is_valid_var_char(char chr)
{
	return (ft_isalnum(chr) || chr == UNDERSCORE_CHR);
}

static char	*extract_braced_var_name(char *str, int *index)
{
	int		start;
	int		end;
	char	*bracket_content;

	(*index)++;
	start = *index;
	end = start;
	while (str[end] && str[end] != BRACKET_CLOSE_CHR)
		end++;
	if (!str[end])
	{
		ft_putendl_fd("minishell: syntax error: missing '}'", STDERR_FILENO);
		return (NULL);
	}
	bracket_content = ft_substr(str, start, end - start);
	*index = end + 1;
	return (bracket_content);
}

char	*extract_var_name_in_brackets_entered(
	char *str, int *index)
{
	int		i;
	char	*var_name;

	var_name = extract_braced_var_name(str, index);
	if (var_name[0] == '\0')
	{
		ft_putstr_fd("minishell: ${}: bad substitution", STDERR_FILENO);
		return (NULL);
	}
	i = 0;
	while (var_name[i])
	{
		if (!is_valid_var_char(var_name[i]))
		{
			ft_putstr_fd("minishell: ${", STDERR_FILENO);
			ft_putstr_fd(var_name, STDERR_FILENO);
			ft_putendl_fd("}: bad substitution", STDERR_FILENO);
			free(var_name);
			return (NULL);
		}
		i++;
	}
	return (var_name);
}
