/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:22:44 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/13 15:40:36 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

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
