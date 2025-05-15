/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:21:26 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 19:28:47 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
 * @brief Checks if the argument is a valid numeric string
 * @param str The string to check
 * @return 1 if valid, 0 otherwise
 * @note Validates that the string contains
	* only digits, with optional leading '+' or '-'
 */
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

/**
 * @brief Handles the exit command in the shell
 * @param param The command arguments and environment variables
 * @return The exit code to be used for the shell
 * @note Prints an error message if the argument is not a valid number
 *       Returns 2 if the argument is not numeric, 1 if too many arguments
 */
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

/**
 * @brief Checks if the command is a built-in command
 * @param cmd The command to check
 * @return 1 if it is a built-in command, 0 otherwise
 * @note Checks for built-in commands: cd, export, unset, exit
 */
int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
}
