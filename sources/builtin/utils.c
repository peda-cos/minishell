/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:37:32 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/03 15:47:40 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	print_invalid_option(char *builtin, char *arg, char *arg_options)
{
	int		index;
	char	*option_chr;

	index = 1;
	if (!arg || !*arg || !arg_options || !*arg_options)
		return ;
	option_chr = ft_strdup(EMPTY_STR);
	while (arg[index])
	{
		if (!ft_strchr(arg_options, arg[index]))
		{
			free(option_chr);
			option_chr = malloc(sizeof(char) * 2);
			option_chr[0] = arg[index];
			option_chr[1] = NULL_CHR;
			break ;
		}
		index++;
	}
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	ft_putstr_fd(builtin, STDERR_FILENO);
	ft_putstr_fd(": -", STDERR_FILENO);
	ft_putstr_fd(option_chr, STDERR_FILENO);
	ft_putendl_fd(": invalid option", STDERR_FILENO);
}
