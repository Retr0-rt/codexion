/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: airkha <airkha@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 00:51:41 by airkha            #+#    #+#             */
/*   Updated: 2026/05/03 00:52:46 by airkha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_int_overflow(const char *str)
{
	long long	value;
	int			i;

	value = 0;
	i = 0;
	while (str[i])
	{
		value = value * 10 + (str[i] - '0');
		if (value > 2147483647)
			return (0);
		i++;
	}
	if (value < 0)
		return (0);
	return (1);
}

static int	is_valid_positive_int(const char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	if (i > 10)
		return (0);
	return (check_int_overflow(str));
}

static int	validate_parsed_values(char **parsed)
{
	int	i;

	i = 0;
	while (i < 7)
	{
		if (!is_valid_positive_int(parsed[i]))
		{
			fprintf(stderr, "Error: arg %d invalid positive integer\n", i + 1);
			return (0);
		}
		i++;
	}
	if (!parsed[7])
	{
		fprintf(stderr, "Error: scheduler must be 'fifo' or 'edf'\n");
		return (0);
	}
	if (strcmp(parsed[7], "fifo") == 0 || strcmp(parsed[7], "edf") == 0)
	{
		return (1);
	}
	fprintf(stderr, "Error: scheduler must be 'fifo' or 'edf'\n");
	return (0);
}

static char	**allocate_and_trim(char **av)
{
	char	**parsed;
	int		i;

	parsed = (char **)malloc(sizeof(char *) * 8);
	if (!parsed)
		return (NULL);
	memset(parsed, 0, sizeof(char *) * 8);
	i = 0;
	while (i < 8)
	{
		parsed[i] = ft_trim(av[i]);
		if (!parsed[i])
		{
			free_parsed_args(parsed);
			return (NULL);
		}
		i++;
	}
	return (parsed);
}

char	**parse_args(int ac, char **av)
{
	char	**parsed;

	if (ac != 8)
	{
		fprintf(stderr, "Error: expected 8 arguments\n");
		return (NULL);
	}
	parsed = allocate_and_trim(av);
	if (!parsed)
		return (NULL);
	if (!validate_parsed_values(parsed))
	{
		free_parsed_args(parsed);
		return (NULL);
	}
	return (parsed);
}
