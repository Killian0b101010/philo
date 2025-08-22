/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <kiteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:54:41 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/22 00:16:24 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	return (ptr);
}

void	free_philo_no_circle(t_philo *philo)
{
	t_philo	*head;
	t_philo	*tmp;

	head = philo;
	while (head != NULL)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
	return ;
}

void	free_philo_circle(t_philo *philo, t_args *args, t_table *table)
{
	t_philo	*current;
	t_philo	*next;
	int		i;

	if (!philo || !args || !table)
		return ;
	current = philo;
	i = 0;
	while (i < args->nb_philo)
	{
		next = current->next;
		pthread_mutex_destroy(&current->fork);
		free(current);
		current = next;
		i++;
	}
	pthread_mutex_destroy(&table->print_mutex);
}
