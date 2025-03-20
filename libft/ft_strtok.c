/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:34:42 by peda-cos          #+#    #+#             */
/*   Updated: 2025/03/10 22:10:16 by jlacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *input, const char *delimiter)
{
	static char	*backup;
	char		*token;

	if (input != NULL)
		backup = input;
	if (backup == NULL)
		return (NULL);
	while (*backup && ft_strchr(delimiter, *backup)) // Ignorar delimitadores iniciais
		backup++;
	if (*backup == '\0')
		return (NULL);
	token = backup;
	while (*backup)
	{
		if (ft_strchr(delimiter, *backup)) // Encontrar o próximo delimitador
		{
			*backup = '\0';
			backup++;
			break ;
		}
		backup++;
	}
	if (*backup == '\0') // Se chegamos ao final da string, definir backup como NULL
		backup = NULL;
	return (token);
}
