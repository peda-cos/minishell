/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:21:26 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/21 14:46:01 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	is_numeric_arg(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\0')
		return (FALSE);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	builtin_exit(t_process_command_args	*param)
{
	int	exit_code;

	if (param->has_fd_redirect_to_stderr && isatty(STDIN_FILENO))
		ft_putendl_fd("exit", STDERR_FILENO);
	else if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", STDOUT_FILENO);
	exit_code = 0;
	if (param->cmd->args[1] == NULL)
		return (exit_code);
	if (!is_numeric_arg(param->cmd->args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(param->cmd->args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		return (2);
	}
	exit_code = ft_atoi(param->cmd->args[1]);
	if (param->cmd->args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	return (exit_code % 256);
}

int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
}
