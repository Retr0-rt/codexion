/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deadlock.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: airkha <airkha@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 18:54:21 by airkha            #+#    #+#             */
/*   Updated: 2026/05/02 18:54:22 by airkha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongles(t_coder *coder)
{
	// 1. Blindly grab the Left dongle
	pthread_mutex_lock(&coder->sys->dongles[coder->left_dongle_id]);
	pthread_mutex_lock(&coder->sys->log_mutex);
	if (!safe_stop_check(coder->sys))
		printf("%lld %d has taken a left dongle\n",
			get_relative_time(coder->sys->start_time), coder->id);
	pthread_mutex_unlock(&coder->sys->log_mutex);
	// 2. Blindly grab the Right dongle
	pthread_mutex_lock(&coder->sys->dongles[coder->right_dongle_id]);
	pthread_mutex_lock(&coder->sys->log_mutex);
	if (!safe_stop_check(coder->sys))
		printf("%lld %d has taken a right dongle\n",
			get_relative_time(coder->sys->start_time), coder->id);
	pthread_mutex_unlock(&coder->sys->log_mutex);
}

void	drop_dongles(t_coder *coder)
{
	// Drop both so the next coder can use them
	pthread_mutex_unlock(&coder->sys->dongles[coder->left_dongle_id]);
	pthread_mutex_lock(&coder->sys->log_mutex);
	if (!safe_stop_check(coder->sys))
		printf("%lld %d has dropped a left dongle\n",
			get_relative_time(coder->sys->start_time), coder->id);
	pthread_mutex_unlock(&coder->sys->log_mutex);

	pthread_mutex_unlock(&coder->sys->dongles[coder->right_dongle_id]);
	if (!safe_stop_check(coder->sys))
		printf("%lld %d has dropped right dongle\n",
			get_relative_time(coder->sys->start_time), coder->id);
	pthread_mutex_unlock(&coder->sys->log_mutex);
}