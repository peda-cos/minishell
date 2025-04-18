/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:16:04 by peda-cos          #+#    #+#             */
/*   Updated: 2025/04/18 19:27:42 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	update_env_variable(char *key, char *value, char ***env)
{
	int		i;
	char	*temp;
	int		key_len;
	char	*new_value;

	key_len = ft_strlen(key);
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, key_len) == 0
			&& (*env)[i][key_len] == '=')
		{
			temp = ft_strjoin(key, "=");
			new_value = ft_strjoin(temp, value);
			free(temp);
			free((*env)[i]);
			(*env)[i] = new_value;
			return ;
		}
		i++;
	}
}
