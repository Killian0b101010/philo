/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <kiteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:54:27 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/22 18:58:50 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

void	*take_fork_two(t_table *table, t_philo *philo)
{
	pthread_mutex_lock(philo->fork_d);
	pthread_mutex_lock(&table->print_mutex);
	if (philo->meals_eaten == table->args->nb_eats || get_stop(table))
	{
		pthread_mutex_unlock(philo->fork_d);
		pthread_mutex_unlock(&table->print_mutex);
		return (NULL);
	}
	printf("%ld %d has taken a fork\n", what_time_is_it() - table->start,
		philo->id);
	pthread_mutex_unlock(&table->print_mutex);
	pthread_mutex_lock(philo->fork_g);
	return (NULL);
}

void	*take_fork(t_table *table, t_philo *philo)
{
	if (philo->meals_eaten == table->args->nb_eats || get_stop(table))
		return (NULL);
	if (!get_stop(table))
	{
		if (philo->meals_eaten == table->args->nb_eats || get_stop(table))
			return (NULL);
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->fork_g);
			pthread_mutex_lock(&table->print_mutex);
			printf("%ld %d has taken a fork\n", what_time_is_it()
				- table->start, philo->id);
			pthread_mutex_unlock(&table->print_mutex);
			pthread_mutex_lock(philo->fork_d);
			if (philo->meals_eaten == table->args->nb_eats || get_stop(table))
				return (NULL);
		}
		else
			take_fork_two(table, philo);
	}
	return (NULL);
}

void	is_eating(t_table *table, t_philo *philo)
{
	if (philo->meals_eaten == table->args->nb_eats || get_stop(table))
		return ;
	pthread_mutex_lock(&table->print_mutex);
	if (!get_stop(table))
		printf("%ld %d is eating\n", what_time_is_it() - table->start,
			philo->id);
	pthread_mutex_unlock(&table->print_mutex);
	pthread_mutex_lock(&philo->protect);
	philo->last_meal = what_time_is_it();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->protect);
	ft_usleep(table->args->time_eat, table);
	pthread_mutex_unlock(philo->fork_d);
	pthread_mutex_unlock(philo->fork_g);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_table	*table;

	free((philo = (t_philo *)arg, table = philo->table, NULL));
	call_one_philo(philo, table);
	while (!get_stop(table))
	{
		if (philo->id % 2 != 0)
			usleep(1000);
		take_fork(table, philo);
		if (get_stop(table))
		{
			t((pthread_mutex_unlock(philo->fork_d),
					pthread_mutex_unlock(philo->fork_g)));
			break ;
		}
		if (get_stop(table))
			break ;
		is_eating(table, philo);
		is_sleeping(table, philo);
		ft_usleep(table->args->time_sleep, table);
		is_thinking(table, philo);
	}
	return (NULL);
}

void	*is_sleeping(t_table *table, t_philo *philo)
{
	if (philo->meals_eaten == table->args->nb_eats || get_stop(table))
		return (NULL);
	if (!get_stop(table))
	{
		pthread_mutex_lock(&table->print_mutex);
		printf("%ld %d is sleeping\n", what_time_is_it() - table->start,
			philo->id);
		pthread_mutex_unlock(&table->print_mutex);
	}
	return (NULL);
}
