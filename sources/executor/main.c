/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:01:28 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/25 08:25:40 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	execute_command(t_command *cmd, char **env, int *last_exit)
{
	int	stdin_backup;
	int	result;

	if (!cmd || !env || !last_exit)
		return ;
	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup < 0)
	{
		perror("dup");
		return ;
	}
	while (cmd)
	{
		result = process_command(cmd, env, last_exit);
		if (result < 0)
			break ;
		cmd = cmd->next;
	}
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
}
