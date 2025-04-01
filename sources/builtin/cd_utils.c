/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:16:04 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/31 22:13:00 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	update_env_variable(char *key, char *value, char ***env)
{
	int		i;
	int		key_len;
	char	*new_value;

	key_len = ft_strlen(key);
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, key_len) == 0
			&& (*env)[i][key_len] == '=')
		{
			new_value = ft_strjoin(key, value);
			free((*env)[i]);
			(*env)[i] = new_value;
			return ;
		}
		i++;
	}
}
