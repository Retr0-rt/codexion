/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: airkha <airkha@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 09:36:54 by airkha            #+#    #+#             */
/*   Updated: 2026/04/22 11:42:08 by airkha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// scheduler can only be "fifo" or "edf" not an int
// i impelemeted this function to excpect an array of strings
// already parsed in the main it doesnt contain any program name
// or invalid output if also enforce the scheduller to be either
// fifo or edf
int	init_system_args(t_system *sys, int ac, char **av)
{
	ac = 0;
	sys->nb_coders = atoi(av[0]);
	sys->time_to_burnout = atoi(av[1]);
	sys->time_to_compile = atoi(av[2]);
	sys->time_to_debug = atoi(av[3]);
	sys->time_to_refactor = atoi(av[4]);
	sys->req_compiles = atoi(av[5]);
	sys->dongle_cooldown = atoi(av[6]);
	if (strcmp(av[7], "fifo") == 0)
		sys->scheduler = SCHED_FIF;
	else
		sys->scheduler = SCHED_EDF;
	return (0);
}

int	init_mutexes(t_system *sys)
{
	int	i;

	i = 0;
	sys->dongles = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* sys->nb_coders);
	if (!sys->dongles)
		return (-1);
	while (i < sys->nb_coders)
	{
		if (pthread_mutex_init(sys->dongles + i, NULL) != 0)
		{
			printf("error initailizing the dongle %d", i + 1);
			return (-1);
		}
		i++;
	}
	if (pthread_mutex_init(&sys->log_mutex, NULL)
		|| pthread_mutex_init(&sys->monitor_mutex, NULL) != 0)
		return (-1);
	return (0);
}

int	init_coders(t_system *sys)
{
	int				i;
	struct timeval	currrent_timestamp;

	gettimeofday(&currrent_timestamp, NULL);
	sys->coders = (t_coder *)malloc(sizeof(t_coder) * sys->nb_coders);
	if (!sys->coders)
		return (-1);
	i = 0;
	while (i < sys->nb_coders)
	{
		sys->coders[i].id = i + 1;
		// we will give last_compile_start 0 for now since the coder didnt compile anytime and 
		// action_compile function will overwrite that 0
		sys->coders[i].last_compile_start = 0; //currrent_timestamp.tv_sec * 1000
			// + currrent_timestamp.tv_usec / 1000;
		sys->coders[i].left_dongle_id = i;
		sys->coders[i].right_dongle_id = (i + 1) % sys->nb_coders;
		sys->coders[i].sys = sys;
		i++;
	}
	return (0);
}

int	start_simulation(t_system *sys)
{
	int				i;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	sys->start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	i = 0;
	while (i < sys->nb_coders)
	{
		if (pthread_create(&sys->coders[i].thread_id, NULL, &coder_routine,
				(void *)&sys->coders[i]))
			return (-1);
		i++;
	}
	if (pthread_create(&sys->monitor_thread, NULL, &monitoring_routine,
			(void *)sys))
		return (-1);
	return (0);
}

int	init_all(t_system *sys, int ac, char **av)
{
	if (init_system_args(sys, ac, av) != 0)
		return (-1);
	if (init_mutexes(sys) != 0)
		return (-1);
	if (init_coders(sys) != 0)
		return (-1);
	if (start_simulation(sys) != 0)
		return (-1);
	return (0);
}


void					*coder_routine(void *arg){
	t_coder *coder = (t_coder *)arg;
	printf("%d ", coder->id);
	return (0);
}

void					*monitoring_routine(void *arg){
	t_system *sys = (t_system *)arg;
	printf("%lld ", sys->start_time);
	return (0);
}