/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:00:10 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 19:34:12 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

/**
 * @brief Retrieves the value of an environment variable
 * @param key The name of the environment variable to search for
 * @param envs The array of environment variables
 * @return The value of the environment variable, or NULL if not found
 * @note Allocates memory for the value and returns a pointer to it
 */
char	*get_env_value(char *key, char **envs)
{
	int		i;
	int		key_len;
	char	*value;

	if (!key || !envs)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (envs[i])
	{
		if (ft_strncmp(envs[i], key, key_len) == 0 && envs[i][key_len] == '=')
		{
			value = ft_strdup(envs[i] + key_len + 1);
			return (value);
		}
		i++;
	}
	return (ft_strdup(""));
}

/**
 * @brief Retrieves the value of the underscore argument
 * @param envs The array of environment variables
 * @return The value of the underscore argument,
	* or the current working directory if not found
 * @note Allocates memory for the value and returns a pointer to it
 */
char	*get_underscore_arg_value(char **envs)
{
	char	*value;

	value = get_env_value("_", envs);
	if (!value)
		return (getcwd(NULL, 0));
	return (value);
}

/**
 * @brief Sets the value of the underscore argument in the command structure
 * @param cmd The command structure containing the arguments
 * @param envs The array of environment variables
 * @return void
 * @note Sets the value of the underscore argument
	* to the last argument of the command
 */
void	set_underscore_arg_value(t_command *cmd, char ***envs)
{
	int		index;
	char	*last_arg_value;

	index = 0;
	while (cmd->args && cmd->args[index])
		index++;
	if (index > 0)
	{
		last_arg_value = cmd->args[index - 1];
		update_env_variable("_", last_arg_value, envs);
	}
}

/**
 * @brief Retrieves the value of a special variable based on its type
 * @param str The string containing the special variable
 * @param index The index of the special variable in the string
 * @param envs The array of environment variables
 * @param last_exit The last exit status
 * @return The value of the special variable, or NULL if not found
 * @note Allocates memory for the value and returns a pointer to it
 */
char	*get_special_variable(char *str,
	int *index, char **envs, int last_exit)
{
	char	*value;

	value = NULL;
	if (ft_isspace(str[*index]))
		value = (ft_strdup("$ "));
	else if (str[*index] == '\0')
		value = (ft_strdup("$"));
	else if (str[*index] == '/')
		value = (ft_strdup("$/"));
	else if (str[*index] == '?')
		value = ft_itoa(last_exit);
	else if (str[*index] == '0')
		value = (ft_strdup("minishell"));
	else if (str[*index] == '_' && str[*index + 1] == '\0')
		value = (get_underscore_arg_value(envs));
	return (value);
}
