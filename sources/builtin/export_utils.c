/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:37:32 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/03 15:19:07 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static void	print_invalid_option(char *arg)
{
	int		index;
	char	*option_chr;

	index = 1;
	option_chr = ft_strdup(EQUAL_STR);
	while (arg[index] && arg[index] != EQUAL_CHR)
	{
		if (!ft_strchr(EXPORT_OPTINS_CHARS, arg[index]))
		{
			free(option_chr);
			option_chr = malloc(sizeof(char) * 2);
			option_chr[0] = arg[index];
			option_chr[1] = NULL_CHR;
			break ;
		}
		index++;
	}
	ft_putstr_fd("Minishell: export: -", STDERR_FILENO);
	ft_putstr_fd(option_chr, STDERR_FILENO);
	ft_putendl_fd(": invalid option", STDERR_FILENO);
}

int	print_invalid_arg(char *arg)
{
	if (arg[0] == DASH_CHR)
	{
		print_invalid_option(arg);
		return (2);
	}
	ft_putstr_fd("Minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (1);
}

int	is_valid_key(char *key)
{
	int	index;

	index = 0;
	if (!key || !*key || (!ft_isalpha(*key) && *key != UNDERSCORE_CHR))
		return (FALSE);
	while (key[index])
	{
		if (!ft_isalnum(key[index]) && key[index] != UNDERSCORE_CHR)
			return (FALSE);
		index++;
	}
	return (TRUE);
}
