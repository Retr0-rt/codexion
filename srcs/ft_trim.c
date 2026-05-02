/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: airkha <airkha@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 17:25:00 by airkha            #+#    #+#             */
/*   Updated: 2026/05/02 17:25:00 by airkha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

/*
** Returns a freshly allocated string with leading and trailing
** whitespace removed.  Returns NULL on allocation failure.
** The caller is responsible for freeing the returned string.
*/
char	*ft_trim(const char *str)
{
	int		start;
	int		end;
	int		len;
	char	*trimmed;
	int		i;

	if (!str)
		return (NULL);
	start = 0;
	while (str[start] && ft_isspace(str[start]))
		start++;
	end = strlen(str);
	while (end > start && ft_isspace(str[end - 1]))
		end--;
	len = end - start;
	trimmed = (char *)malloc(sizeof(char) * (len + 1));
	if (!trimmed)
		return (NULL);
	i = 0;
	while (i < len)
	{
		trimmed[i] = str[start + i];
		i++;
	}
	trimmed[i] = '\0';
	return (trimmed);
}
