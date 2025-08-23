/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <kiteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:00:45 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/23 04:07:21 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

int	check_eat(t_philo *head, t_args *args)
{
	t_philo	*cur;
	int		finished;

	cur = head;
	while (1)
	{
		pthread_mutex_lock(&cur->protect);
		finished = (args->nb_eats > 0 && cur->meals_eaten >= args->nb_eats);
		pthread_mutex_unlock(&cur->protect);
		if (!finished)
			return (0);
		cur = cur->next;
		if (cur == head)
			break ;
	}
	return (1);
}

int	what_time_is_it(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000 + current_time.tv_usec / 1000));
}

int	ft_usleep(int time, t_table *table)
{
	int	start;

	start = what_time_is_it();
	while ((what_time_is_it() - start) < time)
	{
		if (get_stop(table))
			break ;
		usleep(time / 10);
	}
	return (0);
}

int	time_now(t_table *table)
{
	return (what_time_is_it() - table->start);
}
