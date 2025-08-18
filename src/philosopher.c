/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:42:19 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/18 23:40:23 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"
#include <pthread.h>

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
// J'initialise les philo et je met a jour la table pour quel puisse avoir les data et les interpreter
void	init_data(t_philo *philo, t_table *table)
{
	philo->last_meal = table->start;
	philo->fork_g = &philo->fork;
	philo->table = table;
	philo->meals_eaten = 0;
	return ;
}
// Fonction pour un seul philo
// j'ai pas besoin de current
// vu que j'ai que un noeu
// dans ma list c'est la tete principal(head)
t_philo	*create_one_philo(t_table *table)
{
	t_philo	*head;

	head = safe_malloc(sizeof(t_philo));
	if (!head)
		return (NULL);
	pthread_mutex_init(&head->fork, NULL);
	head->id = 1;
	head->next = head;
	init_data(head, table);
	return (head);
}
// Fonction pour plusieurs philo logique un peu differente car je boucle en fonction du nombre de philo comme
// d'habitude je dois initier ma struct t_philo pour creer ma liste
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
	free((head->id = 1, init_data(head, table), NULL));
	i = 2;
	while (i <= table->args->nb_philo)
	{
		current->next = safe_malloc(sizeof(t_philo));
		if (!current->next)
			return ((free_philo_no_circle(head), NULL));
		current = current->next;
		pthread_mutex_init(&current->fork, NULL);
		current->id = i;
		init_data(current, table);
		i++;
	}
	current->next = head;
	return (head);
}
// Fonction qui permet de manage si 1 philo ou plusieurs philo
// je distribue aussi les fourchette a chaque philo grace a un tmp
// pour pas perdre la tete de list.. et a la fin je la return car
// tmp = head c'est juste dire qu'il pointe sur la mm adresse sauf que je me
// deplace avec tmp et pas head pour pas casser la list original head.
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

int	what_time_is_it(t_table *table)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	table->now = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (table->now);
}

void	*routine(void *arg)
{
	t_table	*table;
	t_philo	*philo;

	philo = (t_philo *)arg;
	(void)arg;
	while (!table->stop)
	{
		if (table->head->id % 2 == 0)
		{
			pthread_mutex_lock(table->head->fork_d);
			printf("Philo %d has taken a fork\n", table->head->id);
			pthread_mutex_unlock(table->head->fork_d);
		}
		else
		{
			pthread_mutex_lock(table->head->fork_g);
			printf("Philo %d has taken a fork\n", table->head->id);
			pthread_mutex_unlock(table->head->fork_d);
		}
	}
	return (NULL);
}

void	create_thread(t_table *table, t_philo *philo)
{
	int		i;
	t_philo	*tmp;

	tmp = philo;
	i = 0;
	while (table->args->nb_philo > i)
	{
		pthread_create(&tmp->thread, NULL, routine, tmp);
		tmp = tmp->next;
		i++;
	}
	pthread_join(philo->thread, NULL);
}

void	monitor(t_table *table)
{
}

void	manage(t_args *args, char **argv, t_table *table)
{
	ft_convert_args(args, argv);
	table->args = args;
	table->stop = 0;
	pthread_mutex_init(&table->print_mutex, NULL);
	pthread_mutex_init(&table->stop_mutex, NULL);
	table->head = init_table(args, table);
	create_thread(table, table->head);
}

int	main(int ac, char **argv)
{
	t_args	args;
	t_table	table;

	(void)argv;
	if (ac < 3)
		return (write(2, ERROR, 1));
	table.start = what_time_is_it(&table);
	manage(&args, argv, &table);
}
