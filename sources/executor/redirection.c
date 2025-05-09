/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:15:40 by peda-cos          #+#    #+#             */
/*   Updated: 2025/05/04 18:56:08 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Sets up output redirection for a command
 * @param param Pointer to the command arguments structure
 * @return 0 on success, -1 on failure
 * @note Determines appropriate file flags based on command properties:
 *       - Uses O_APPEND flag for append redirections (>>)
 *       - Uses O_TRUNC flag for standard redirections (>)
 *       - Always includes O_WRONLY and O_CREAT flags
 */
int	setup_output_redirection(t_process_command_args *param)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (param->cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	if (set_redirection_to_files(param, &flags) < 0)
		return (-1);
	return (0);
}
