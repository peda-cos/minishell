/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:20:01 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/27 23:57:57 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	is_valid_n_option(char *arg)
{
	int	j;

	j = 1;
	if (!ft_strncmp(arg, "-n", 2))
	{
		while (arg[j])
		{
			if (arg[j] != 'n')
				return (0);
			j++;
		}
		return (1);
	}
	return (0);
}

static void	print_echo_args(char **args, int start_idx)
{
	int	i;

	i = start_idx;
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
}

int	builtin_echo(char **args, int *last_exit)
{
	int	i;
	int	newline;

	(void)last_exit;
	if (!args)
		return (1);
	i = 1;
	newline = 1;
	while (args[i] && is_valid_n_option(args[i]))
	{
		newline = 0;
		i++;
	}
	print_echo_args(args, i);
	if (newline)
		ft_putendl_fd("", STDOUT_FILENO);
	return (0);
}

// int main(void)
// {
// 	char *args[] = { "echo", "-n", "Hello", "world", NULL };
// 	int last_exit = 0;
// 	builtin_echo(args, &last_exit);
// 	return (0);
// }