/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:37:32 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/03 15:47:55 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	print_invalid_arg(char *arg)
{
	if (arg[0] == DASH_CHR)
	{
		print_invalid_option("export", arg, EXPORT_OPTINS_CHARS);
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
