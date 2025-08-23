/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <kiteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:57:49 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/23 04:19:36 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

void	t(int nb)
{
	(void)nb;
}

void	*philo_is_dead(t_philo *current, t_table *table, int now)
{
	while (1)
	{
		now = what_time_is_it();
		pthread_mutex_lock(&current->protect);
		if ((now - current->last_meal) > table->args->time_dead)
		{
			pthread_mutex_lock(&table->print_mutex);
			set_stop(table, 1);
			if (get_stop(table))
				printf("%ld %d died\n", now - table->start, current->id);
			pthread_mutex_unlock(&table->print_mutex);
			pthread_mutex_unlock(&current->protect);
			return (NULL);
		}
		pthread_mutex_unlock(&current->protect);
		current = current->next;
		if (current == table->head)
			break ;
	}
	return (NULL);
}

void	init_data(t_philo *philo, t_table *table)
{
	philo->last_meal = table->start;
	philo->fork_g = &philo->fork;
	philo->table = table;
	philo->meals_eaten = 0;
	return ;
}

t_philo	*create_one_philo(t_table *table)
{
	t_philo	*head;

	head = safe_malloc(sizeof(t_philo));
	if (!head)
		return (NULL);
	pthread_mutex_init(&head->protect, NULL);
	pthread_mutex_init(&head->fork, NULL);
	head->id = 1;
	head->next = head;
	init_data(head, table);
	return (head);
}

void	*is_thinking(t_table *table, t_philo *philo)
{
	int	done;

	pthread_mutex_lock(&philo->protect);
	done = (philo->meals_eaten == table->args->nb_eats);
	pthread_mutex_unlock(&philo->protect);
	if (done || get_stop(table))
		return (0);
	if (!get_stop(table))
	{
		pthread_mutex_lock(&table->print_mutex);
		if (!get_stop(table))
			printf("%ld %d is thinking\n", what_time_is_it() - table->start,
				philo->id);
		pthread_mutex_unlock(&table->print_mutex);
	}
	return (NULL);
}
