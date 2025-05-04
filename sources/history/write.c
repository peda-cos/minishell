/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:22:44 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 19:02:40 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/**
	* @brief Opens the history file for writing
	* @return int File descriptor for the opened file, or -1 on error
	* @note Creates the file if it doesn't exist, and truncates it if it does
 */
int	save_command_to_history(char *cmd)
{
	int	fd;

	if (!cmd || !*cmd)
		return (1);
	fd = open_history_file_write();
	if (fd < 0)
		return (1);
	ft_putendl_fd(cmd, fd);
	close(fd);
	return (0);
}
