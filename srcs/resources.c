/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resources.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: airkha <airkha@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 18:54:02 by airkha            #+#    #+#             */
/*   Updated: 2026/05/02 18:54:03 by airkha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	log_taking_a_dongle(t_coder *coder)
{
	pthread_mutex_lock(&coder->sys->log_mutex);
	if (!safe_stop_check(coder->sys))
		printf("%lld %d has taken a dongle\n",
			get_relative_time(coder->sys->start_time), coder->id);
	pthread_mutex_unlock(&coder->sys->log_mutex);
}

void	acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	long long	priority;
	long long	wait_time;

	if (coder->sys->scheduler == FIFO)
		priority = get_relative_time(coder->sys->start_time);
	else
		priority = coder->last_compile_start + coder->sys->time_to_burnout;
	pthread_mutex_lock(&dongle->mutex);
	heap_push(dongle->queue, coder->id, priority);
	while (!safe_stop_check(coder->sys)
		&& (heap_peek(dongle->queue) != coder->id || dongle->is_taken == 1))
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	if (safe_stop_check(coder->sys))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	heap_pop(dongle->queue);
	dongle->is_taken = 1;
	wait_time = dongle->available_at;
	wait_time -= get_relative_time(coder->sys->start_time);
	pthread_mutex_unlock(&dongle->mutex);
	if (wait_time > 0)
		ft_msleep(wait_time, coder->sys);
	log_taking_a_dongle(coder);
}
void	take_dongles(t_coder *coder)
{
	t_dongle	*first_dongle;
	t_dongle	*second_dongle;

	if (coder->id % 2 == 0)
	{
		first_dongle = &coder->sys->dongles[coder->right_dongle_id];
		second_dongle = &coder->sys->dongles[coder->left_dongle_id];
	}
	else
	{
		first_dongle = &coder->sys->dongles[coder->left_dongle_id];
		second_dongle = &coder->sys->dongles[coder->right_dongle_id];
	}
	acquire_dongle(coder, first_dongle);
	if (safe_stop_check(coder->sys))
		return ;
	acquire_dongle(coder, second_dongle);
}

/* Dijkstra implementation */

// void take_dongles(t_coder *coder)
// {
//     t_dongle *low_id_dongle;
//     t_dongle *high_id_dongle;
//     if(coder->left_dongle_id < coder->right_dongle_id)
//     {
//         low_id_dongle = &coder->sys->dongles[coder->left_dongle_id];
//         high_id_dongle = &coder->sys->dongles[coder->right_dongle_id];
//     }
//     else
//     {
//         low_id_dongle = &coder->sys->dongles[coder->right_dongle_id];
//         high_id_dongle = &coder->sys->dongles[coder->left_dongle_id];
//     }
//     acquire_dongle(coder, low_id_dongle);
//     if (safe_stop_check(coder->sys))
//         return ;
//     acquire_dongle(coder, high_id_dongle);
// }

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_taken = 0;
	dongle->available_at = get_relative_time(coder->sys->start_time);
	dongle->available_at += coder->sys->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
void	drop_dongles(t_coder *coder)
{
	release_dongle(coder, &coder->sys->dongles[coder->left_dongle_id]);
	release_dongle(coder, &coder->sys->dongles[coder->right_dongle_id]);
}