/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: airkha <airkha@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 18:54:17 by airkha            #+#    #+#             */
/*   Updated: 2026/05/03 15:46:03 by airkha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_parsed_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (i < 8)
	{
		if (args[i])
			free(args[i]);
		i++;
	}
	free(args);
}

void	cleanup_system(t_system *sys)
{
	int	i;

	i = 0;
	pthread_join(sys->monitor_thread, NULL);
	while (i < sys->nb_coders)
	{
		pthread_mutex_lock(&sys->dongles[i].mutex);
		pthread_cond_broadcast(&sys->dongles[i].cond);
		pthread_mutex_unlock(&sys->dongles[i].mutex);
		i++;
	}
	i = 0;
	while (i < sys->nb_coders)
	{
		pthread_join(sys->coders[i].thread_id, NULL);
		sys->stop_flag = 1;
		i++;
	}
	free_all_mutexes_and_cond_vars(sys);
	pthread_mutex_destroy(&sys->state_mutex);
	pthread_mutex_destroy(&sys->log_mutex);
	pthread_cond_destroy(&sys->start_gun_cv);
	free(sys->dongles);
	free(sys->coders);
}

void	free_all_mutexes_and_cond_vars(t_system *sys)
{
	int	i;

	i = 0;
	while (i < sys->nb_coders)
	{
		pthread_mutex_destroy(&sys->dongles[i].mutex);
		pthread_cond_destroy(&sys->dongles[i].cond);
		free(sys->dongles[i].queue->array);
		free(sys->dongles[i].queue);
		pthread_mutex_destroy(&sys->coders[i].coder_mutex);
		i++;
	}
}
