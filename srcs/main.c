/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: airkha <airkha@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 17:25:00 by airkha            #+#    #+#             */
/*   Updated: 2026/05/02 17:25:00 by airkha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** ./codexion number_of_coders time_to_burnout time_to_compile
**            time_to_debug time_to_refactor number_of_compiles_required
**            dongle_cooldown scheduler
*/
int	main(int ac, char **av)
{
	t_system sys;
	char **parsed;

	parsed = parse_args(ac - 1, av + 1);
	if (!parsed)
		return (1);
	if (init_all(&sys, ac, parsed) != 0)
	{
		fprintf(stderr, "Error: Initialization failed\n");
		free_parsed_args(parsed);
		return (1);
	}
	cleanup_system(&sys);
	free_parsed_args(parsed);
	return (0);
}