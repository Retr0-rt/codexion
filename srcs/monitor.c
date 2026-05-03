/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: airkha <airkha@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 18:53:54 by airkha            #+#    #+#             */
/*   Updated: 2026/05/02 23:29:59 by airkha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_someone_burned_out(t_system *sys)
{
	int			i;
	long long	current_timestamp;

	i = 0;
	while (i < sys->nb_coders)
	{
		current_timestamp = get_relative_time(sys->start_time);
		pthread_mutex_lock(&sys->coders[i].coder_mutex);
		if (current_timestamp
			- sys->coders[i].last_compile_start >= sys->time_to_burnout)
		{
			pthread_mutex_lock(&sys->state_mutex);
			sys->stop_flag = 1;
			pthread_mutex_unlock(&sys->state_mutex);
			pthread_mutex_lock(&sys->log_mutex);
			printf("%lld %d burned out\n", current_timestamp,
				sys->coders[i].id);
			pthread_mutex_unlock(&sys->log_mutex);
			pthread_mutex_unlock(&sys->coders[i].coder_mutex);
			return (1);
		}
		pthread_mutex_unlock(&sys->coders[i].coder_mutex);
		i++;
	}
	return (0);
}

int	won(t_system *sys)
{
	int	i;

	i = 0;
	while (i < sys->nb_coders)
	{
		pthread_mutex_lock(&sys->coders[i].coder_mutex);
		if (sys->coders[i].compiles_done < sys->req_compiles)
		{
			pthread_mutex_unlock(&sys->coders[i].coder_mutex);
			return (0);
		}
		pthread_mutex_unlock(&sys->coders[i].coder_mutex);
		i++;
	}
	pthread_mutex_lock(&sys->state_mutex);
	sys->stop_flag = 1;
	pthread_mutex_unlock(&sys->state_mutex);
	pthread_mutex_lock(&sys->log_mutex);
	printf("All the coders achieved %d compiles\n", sys->req_compiles);
	pthread_mutex_unlock(&sys->log_mutex);
	return (1);
}

void	*monitoring_routine(void *arg)
{
	t_system	*sys;

	sys = (t_system *)arg;
	while (1)
	{
		if (is_someone_burned_out(sys) || won(sys))
			break ;
		usleep(300);
	}
	return (NULL);
}
