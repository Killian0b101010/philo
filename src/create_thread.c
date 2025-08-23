/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <kiteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:54:32 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/23 04:06:37 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

t_philo	*create_multiple_philo(t_table *table)
{
	int		i;
	t_philo	*current;
	t_philo	*head;

	head = safe_malloc(sizeof(t_philo));
	current = head;
	if (!head)
		return (NULL);
	pthread_mutex_init(&current->fork, NULL);
	pthread_mutex_init(&current->protect, NULL);
	free((head->id = 1, init_data(head, table), NULL));
	i = 2;
	while (i <= table->args->nb_philo)
	{
		current->next = safe_malloc(sizeof(t_philo));
		if (!current->next)
			return ((free_philo_no_circle(head), NULL));
		current = current->next;
		pthread_mutex_init(&current->fork, NULL);
		pthread_mutex_init(&current->protect, NULL);
		current->id = i;
		init_data(current, table);
		i++;
	}
	return ((current->next = head, head));
}

t_philo	*init_table(t_args *args, t_table *table)
{
	t_philo	*head;
	t_philo	*tmp;
	int		j;

	if (args->nb_philo == 1)
		head = create_one_philo(table);
	else
		head = create_multiple_philo(table);
	j = 0;
	tmp = head;
	while (j < args->nb_philo)
	{
		tmp->fork_d = &tmp->next->fork;
		tmp = tmp->next;
		j++;
	}
	return (head);
}

void	call_one_philo(t_philo *philo, t_table *table)
{
	if (table->args->nb_philo == 1)
		one_philo(philo, table);
}

void	create_thread(t_table *table, t_philo *philo)
{
	int		i;
	t_philo	*tmp;

	tmp = philo;
	i = 0;
	if (pthread_create(&table->monitor_thread, NULL, monitor,
			(void *)table) != 0)
		return ;
	while (i < table->args->nb_philo)
	{
		if (pthread_create(&tmp->thread, NULL, routine, tmp) != 0)
			tmp->thread = 0;
		tmp = tmp->next;
		i++;
	}
	tmp = philo;
	i = 0;
	while (i < table->args->nb_philo)
	{
		if (tmp->thread)
			pthread_join(tmp->thread, NULL);
		tmp = tmp->next;
		i++;
	}
	pthread_join(table->monitor_thread, NULL);
}

void	one_philo(t_philo *philo, t_table *table)
{
	if (table->args->nb_philo == 1)
	{
		pthread_mutex_lock(&philo->fork);
		pthread_mutex_lock(&table->print_mutex);
		printf("%lu %d has taken a fork\n", what_time_is_it() - table->start,
			philo->id);
		pthread_mutex_unlock(&table->print_mutex);
		pthread_mutex_lock(&table->print_mutex);
		ft_usleep(table->args->time_dead, table);
		printf("%lu %d died\n", what_time_is_it() - table->start, philo->id);
		set_stop(table, 1);
		pthread_mutex_unlock(&table->print_mutex);
		pthread_mutex_unlock(&philo->fork);
		return ;
	}
}
