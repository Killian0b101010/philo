/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <kiteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:42:19 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/22 21:06:49 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

int	ft_convert_args(int *ac, t_args *args, char **argv)
{
	args->nb_philo = is_valid_number(argv[1]);
	if (args->nb_philo <= 0)
		return ((write(2, ERROR_VALUE, strlen(ERROR_VALUE))), 0);
	args->time_dead = is_valid_number(argv[2]);
	if (args->time_dead <= 0)
		return ((write(2, ERROR_VALUE, strlen(ERROR_VALUE))), 0);
	args->time_eat = is_valid_number(argv[3]);
	if (args->time_eat <= 0)
		return ((write(2, ERROR_VALUE, strlen(ERROR_VALUE))), 0);
	args->time_sleep = is_valid_number(argv[4]);
	if (args->time_sleep <= 0)
		return ((write(2, ERROR_VALUE, strlen(ERROR_VALUE))), 0);
	if (*ac == 5)
		args->nb_eats = -1;
	else if (*ac == 6)
		args->nb_eats = is_valid_number(argv[5]);
	if (args->nb_eats <= 0 && *ac == 6)
		return ((write(2, ERROR_VALUE, strlen(ERROR_VALUE))), 0);
	return (1);
}

void	*monitor(void *arg)
{
	t_table	*table;
	t_philo	*current;
	long	now;

	now = 0;
	table = (t_table *)arg;
	while (!get_stop(table))
	{
		current = table->head;
		philo_is_dead(current, table, now);
		if (check_eat(table->head, table->args))
		{
			pthread_mutex_lock(&table->print_mutex);
			set_stop(table, 1);
			printf("All philosophers have eaten enough\n");
			pthread_mutex_unlock(&table->print_mutex);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

t_philo	*manage(int *ac, t_args *args, char **argv, t_table *table)
{
	if ((ft_convert_args(ac, args, argv) == 1))
	{
		table->args = args;
		table->stop = 0;
		pthread_mutex_init(&table->print_mutex, NULL);
		pthread_mutex_init(&table->stop_mutex, NULL);
		table->head = init_table(args, table);
		create_thread(table, table->head);
		return (table->head);
	}
	return (NULL);
}

int	main(int ac, char **argv)
{
	t_args	args;
	t_table	table;
	t_philo	*philo;

	(void)argv;
	if (ac < 5)
		return (write(2, ERROR, strlen(ERROR)));
	if (ac > 6)
		return (write(2, ERROR, strlen(ERROR)));
	if (ac == 5 || ac == 6)
	{
		table.start = what_time_is_it();
		philo = manage(&ac, &args, argv, &table);
		free_philo_circle(philo, &args, &table);
	}
}
