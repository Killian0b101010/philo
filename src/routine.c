/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <kiteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:54:27 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/22 23:50:25 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

int	take_fork(t_table *table, t_philo *philo)
{
	int	done;

	pthread_mutex_lock(&philo->protect);
	done = (philo->meals_eaten == table->args->nb_eats);
	pthread_mutex_unlock(&philo->protect);
	if (done || get_stop(table))
		return (0);
	pthread_mutex_lock(philo->fork_g);
	pthread_mutex_lock(&table->print_mutex);
	if (!get_stop(table) && !done)
		printf("%ld %d has taken a fork\n", what_time_is_it() - table->start,
			philo->id);
	pthread_mutex_unlock(&table->print_mutex);
	pthread_mutex_lock(philo->fork_d);
	if (get_stop(table) || done)
	{
		pthread_mutex_unlock(philo->fork_g);
		pthread_mutex_unlock(philo->fork_d);
		return (0);
	}
	return (1);
}

int	take_fork_two(t_table *table, t_philo *philo)
{
	int	done;

	pthread_mutex_lock(&philo->protect);
	done = (philo->meals_eaten == table->args->nb_eats);
	pthread_mutex_unlock(&philo->protect);
	if (done || get_stop(table))
		return (0);
	pthread_mutex_lock(philo->fork_d);
	pthread_mutex_lock(&table->print_mutex);
	if (!get_stop(table) && !done)
		printf("%ld %d has taken a fork\n", what_time_is_it() - table->start,
			philo->id);
	pthread_mutex_unlock(&table->print_mutex);
	pthread_mutex_lock(philo->fork_g);
	if (get_stop(table) || done)
	{
		pthread_mutex_unlock(philo->fork_d);
		pthread_mutex_unlock(philo->fork_g);
		return (0);
	}
	return (1);
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
		if ((philo->id % 2 == 0 && !take_fork(table, philo)) || (philo->id
				% 2 != 0 && !take_fork_two(table, philo)))
			return (NULL);
		call_routine(table, philo);
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
		if (!get_stop(table))
			printf("%ld %d is sleeping\n", what_time_is_it() - table->start,
				philo->id);
		pthread_mutex_unlock(&table->print_mutex);
	}
	return (NULL);
}
