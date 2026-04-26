/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: airkha <airkha@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 09:36:54 by airkha            #+#    #+#             */
/*   Updated: 2026/04/25 23:48:19 by airkha           ###   ########.fr       */
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
	sys->ready_flag = 0;
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
			return (-1);
		i++;
	}
	if (pthread_mutex_init(&sys->log_mutex, NULL) != 0
		|| pthread_mutex_init(&sys->state_mutex, NULL) != 0
		|| pthread_cond_init(&sys->start_gun_cv, NULL) != 0)
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
		if (pthread_mutex_init(&sys->coders[i].coder_mutex, NULL) != 0)
			return (-1);
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

	i = 0;
	while (i < sys->nb_coders)
	{
		if (pthread_create(&sys->coders[i].thread_id, NULL, &coder_routine,
				(void *)&sys->coders[i]))
			return (-1);
		i++;
	}
	pthread_mutex_lock(&sys->state_mutex);
	sys->start_time = get_relative_time(0);
	sys->stop_flag = 0;
	sys->ready_flag = 1;
	pthread_cond_broadcast(&sys->start_gun_cv);
	pthread_mutex_unlock(&sys->state_mutex);
	
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
