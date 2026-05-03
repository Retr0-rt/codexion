/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: airkha <airkha@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 18:54:12 by airkha            #+#    #+#             */
/*   Updated: 2026/05/03 15:46:03 by airkha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_relative_time(long long start_time)
{
	struct timeval	tv;
	long long		relative_time;

	gettimeofday(&tv, NULL);
	relative_time = tv.tv_sec * 1000 + tv.tv_usec / 1000 - start_time;
	return (relative_time);
}

void	ft_msleep(int time_to_sleep, t_system *sys)
{
	long long	start_time;

	start_time = get_relative_time(0);
	while ((get_relative_time(0) - start_time) < time_to_sleep)
	{
		if (safe_stop_check(sys))
			break ;
		usleep(500);
	}
}

void	wait_for_start(t_system *sys)
{
	pthread_mutex_lock(&sys->state_mutex);
	while (sys->ready_flag == 0)
	{
		pthread_cond_wait(&sys->start_gun_cv, &sys->state_mutex);
	}
	pthread_mutex_unlock(&sys->state_mutex);
}
