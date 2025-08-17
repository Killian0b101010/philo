/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:42:19 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/17 19:39:29 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

void	ft_convert_args(t_args *args, char **argv)
{
	args->nb_philo = atol(argv[1]);
	if (args->nb_philo <= 0)
		return ;
	args->time_dead = atol(argv[2]);
	args->time_eat = atol(argv[3]);
	args->time_sleep = atol(argv[4]);
}

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

void	init_data(t_philo *philo, t_table *table, t_philo *head)
{
	philo->last_meal = table->start;
	philo->fork_g = &philo->fork;
	philo->table = table;
	philo->meals_eaten = 0;
	return ;
}

t_philo	*init_table(t_args *args, t_table *table)
{
	int		i;
	t_philo	*head;
	t_philo	*current;
	t_philo	*tmp;
	int		j;

	head = safe_malloc(sizeof(t_philo));
	if (!head)
		return (NULL);
	pthread_mutex_init(&head->fork, NULL);
	current = head;
	// cas ou 1 philo on init head;
	if (args->nb_philo == 1)
	{
		current->next = head;
		pthread_mutex_init(&current->fork, NULL);
		head->id = 1;
		init_data(head, table, head);
		return (head);
	}
	// cas ou plusieurs philo
	pthread_mutex_init(&current->fork, NULL);
	head->id = 1;
	init_data(head, table, head);
	i = 2; // init a 2 car philo 1 gerer par defaut
			// Plusieurs philo
	while (i <= args->nb_philo)
	{
		current->next = safe_malloc(sizeof(t_philo));
		if (!current->next)
		{
			free_philo_no_circle(head);
			return (NULL);
		}
		current = current->next;
		pthread_mutex_init(&current->fork, NULL);
		current->id = i;
		init_data(current, table, head);
		i++;
	}
	current->next = head;
	tmp = head;
	j = 0;
	// distrib des fourchettes
	while (j < args->nb_philo)
	{
		tmp->fork_d = &tmp->next->fork;
		tmp = tmp->next;
		j++;
	}
	return (head);
}

void	manage_table(t_args *args, char **argv, t_table *table)
{
	t_philo	*philo;

	ft_convert_args(args, argv);
	philo = init_table(args, table);
}

int	main(int ac, char **argv)
{
	t_args			args;
	t_table			table;
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	table.start = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	(void)argv;
	if (ac < 3)
		return (write(2, ERROR, 1));
	manage_table(&args, argv, &table);
}
