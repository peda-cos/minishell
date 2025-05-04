/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:12:37 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/03 18:45:18 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Frees the memory allocated for a NULL-terminated array of strings
 * @param split The array of strings to be freed
 * @return void
 * @note Handles NULL input safely
 */
void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

/**
	* @brief Joins a directory and a command into a full path
	* @param dir The directory to join with the command
	* @param cmd The command to join with the directory
	* @return The full path as a dynamically allocated string, or NULL on failure
	* @note Allocates memory for the full path and frees it after use
	*/
static char	*join_path(char *dir, char *cmd)
{
	char	*temp;
	char	*full;

	if (!dir || !cmd)
		return (NULL);
	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full = ft_strjoin(temp, cmd);
	free(temp);
	return (full);
}

/**
	* @brief Searches for the command in the directories specified in PATH
	* @param paths The array of directories to search in
	* @param cmd The command to search for
	* @return The full path of the command if found, or NULL if not found
	* @note Frees the paths array after use
	*/
static char	*search_in_paths(char **paths, char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = join_path(paths[i], cmd);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

static char	*search_in_current_dir(char *cmd)
{
	char	*current_dir;
	char	*full_path;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (NULL);
	full_path = join_path(current_dir, cmd);
	free(current_dir);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*find_executable(char *cmd, char **env)
{
	char	*path_env;
	char	**paths;
	int		i;

	if (!cmd || !env)
		return (NULL);
	if (cmd[0] == '/'
		|| (cmd[0] == '.' && cmd[1] == '/')
		|| (cmd[0] == '.' && cmd[1] == '.' && cmd[2] == '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (search_in_current_dir(cmd));
	path_env = env[i] + 5;
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd));
}
