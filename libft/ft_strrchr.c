/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 10:34:42 by peda-cos          #+#    #+#             */
/*   Updated: 2024/10/09 07:01:54 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*last;
	char	ch;
	size_t	i;

	ch = (char)c;
	last = NULL;
	i = 0;
	while (s[i])
	{
		if (s[i] == ch)
			last = (char *)&s[i];
		i++;
	}
	if (ch == '\0')
		return ((char *)&s[i]);
	return (last);
}
