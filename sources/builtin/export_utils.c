/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:37:32 by jlacerda          #+#    #+#             */
/*   Updated: 2025/05/03 14:15:06 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	print_invalid_arg(char *arg)
{
	ft_putstr_fd("Minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
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
