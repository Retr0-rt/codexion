/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: airkha <airkha@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 17:25:00 by airkha            #+#    #+#             */
/*   Updated: 2026/05/02 17:25:00 by airkha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** Validates that a trimmed string represents a positive integer:
**   - Non-empty
**   - All characters are digits (no sign prefix, no spaces)
**   - Value fits in int range and is > 0
** Returns 1 on success, 0 on failure.
*/
static int	is_valid_positive_int(const char *str)
{
	int			i;
	long long	value;

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
	value = 0;
	i = 0;
	while (str[i])
	{
		value = value * 10 + (str[i] - '0');
		if (value > 2147483647)
			return (0);
		i++;
	}
	if (value <= 0)
		return (0);
	return (1);
}

static int	validate_scheduler(const char *str)
{
	if (!str)
		return (0);
	if (strcmp(str, "fifo") == 0 || strcmp(str, "edf") == 0)
		return (1);
	return (0);
}

/*
** Frees the parsed argument array (8 trimmed strings).
*/
void	free_parsed_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (i < 8)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

/*
** Parses and validates ac/av (already skipping program name).
** Expects exactly 8 arguments:
**   [0] number_of_coders       (positive int)
**   [1] time_to_burnout        (positive int)
**   [2] time_to_compile        (positive int)
**   [3] time_to_debug          (positive int)
**   [4] time_to_refactor       (positive int)
**   [5] number_of_compiles_req (positive int)
**   [6] dongle_cooldown        (positive int)
**   [7] scheduler              ("fifo" or "edf")
**
** Returns a freshly allocated char** of 8 trimmed strings on success.
** Returns NULL on any validation failure (with error printed to stderr).
*/
char	**parse_args(int ac, char **av)
{
	char	**parsed;
	int		i;

	if (ac != 8)
		return (fprintf(stderr, "Error: expected 8 arguments\n"), NULL);
	parsed = (char **)malloc(sizeof(char *) * 8);
	if (!parsed)
		return (NULL);
	memset(parsed, 0, sizeof(char *) * 8);
	i = 0;
	while (i < 8)
	{
		parsed[i] = ft_trim(av[i]);
		if (!parsed[i])
			return (free_parsed_args(parsed), NULL);
		i++;
	}
	i = 0;
	while (i < 7)
	{
		if (!is_valid_positive_int(parsed[i]))
			return (fprintf(stderr,
							"Error: argument %d is not a valid"
							" positive integer\n",
							i + 1),
					free_parsed_args(parsed),
					NULL);
		i++;
	}
	if (!validate_scheduler(parsed[7]))
		return (fprintf(stderr,
						"Error: scheduler must be 'fifo'"
						" or 'edf'\n"),
				free_parsed_args(parsed),
				NULL);
	return (parsed);
}
