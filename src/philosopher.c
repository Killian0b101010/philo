/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:42:19 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/20 05:38:01 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

void	ft_convert_args(int *ac, t_args *args, char **argv)
{
	args->nb_philo = atol(argv[1]);
	if (args->nb_philo <= 0)
		return ;
	args->time_dead = atol(argv[2]);
	args->time_eat = atol(argv[3]);
	args->time_sleep = atol(argv[4]);
	if (*ac == 5)
		args->nb_eats = -1;
	else if (*ac == 6)
		args->nb_eats = atol(argv[5]);
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
		pthread_mutex_destroy(&current->fork); // détruire le fork du philo
		free(current);
		current = next;
		i++;
	}
	pthread_mutex_destroy(&table->print_mutex); // mutex global
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
// check_eat
// time eat
// free_all (destroy_mutex, free)
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
	return (1); // tous ont mangé assez
}
int	what_time_is_it(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000 + current_time.tv_usec / 1000));
}

int	check_time(t_philo *philo, t_args *args)
{
	t_philo	*current;

	current = philo;
	while (1)
	{
		if (what_time_is_it() - current->last_meal > args->time_dead)
			return (current->id);
		current = current->next;
		if (current == philo)
			break ;
	}
	return (0);
}

int	ft_usleep(int time)
{
	int	start;

	start = what_time_is_it();
	while ((what_time_is_it() - start) < time)
		usleep(time / 10);
	return (0);
}
int	time_now(t_table *table)
{
	return (what_time_is_it() - table->start);
}

void	one_philo(t_philo *philo, t_table *table)
{
	if (table->args->nb_philo == 1)
	{
		pthread_mutex_lock(&philo->fork);
		printf("Philo 1 has taken a fork");
		ft_usleep(table->args->time_dead);
		printf("Philo 1 is dead");
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_destroy(&philo->fork);
		return ;
	}
}
int	get_stop(t_table *table)
{
	int	value;

	pthread_mutex_lock(&table->stop_mutex);
	value = table->stop;
	pthread_mutex_unlock(&table->stop_mutex);
	return (value);
}

void	set_stop(t_table *table, int value)
{
	pthread_mutex_lock(&table->stop_mutex);
	table->stop = value;
	pthread_mutex_unlock(&table->stop_mutex);
}
void	*routine(void *arg)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)arg;
	table = philo->table;
	if (table->args->nb_philo == 1)
	{
		one_philo(philo, table);
		return (NULL);
	}
	while (!get_stop(table))
	{
		// prendre les fourchettes
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->fork_g);
			pthread_mutex_lock(philo->fork_d);
		}
		else
		{
			pthread_mutex_lock(philo->fork_d);
			pthread_mutex_lock(philo->fork_g);
		}
		// re-check après avoir pris les fourchettes
		if (get_stop(table))
		{
			pthread_mutex_unlock(philo->fork_d);
			pthread_mutex_unlock(philo->fork_g);
			break ;
		}
		// manger
		pthread_mutex_lock(&table->print_mutex);
		if (!get_stop(table))
			printf("%ld Philo %d is eating\n", what_time_is_it() - table->start,
				philo->id);
		pthread_mutex_unlock(&table->print_mutex);
		pthread_mutex_lock(&philo->protect);
		philo->last_meal = what_time_is_it();
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->protect);
		ft_usleep(table->args->time_eat);
		pthread_mutex_unlock(philo->fork_d);
		pthread_mutex_unlock(philo->fork_g);
		if (get_stop(table))
			break ;
		// dormir
		pthread_mutex_lock(&table->print_mutex);
		printf("%ld Philo %d is sleeping\n", what_time_is_it() - table->start,
			philo->id);
		pthread_mutex_unlock(&table->print_mutex);
		ft_usleep(table->args->time_sleep);
		if (get_stop(table))
			break ;
		// penser
		pthread_mutex_lock(&table->print_mutex);
		printf("%ld Philo %d is thinking\n", what_time_is_it() - table->start,
			philo->id);
		pthread_mutex_unlock(&table->print_mutex);
	}
	return (NULL);
}
void	*monitor(void *arg)
{
	t_table	*table;
	t_philo	*current;
	long	now;

	table = (t_table *)arg;
	while (!get_stop(table))
	{
		current = table->head;
		while (1)
		{
			now = what_time_is_it();
			pthread_mutex_lock(&current->protect);
			if ((now - current->last_meal) >= table->args->time_dead)
			{
				pthread_mutex_lock(&table->print_mutex);
				printf("%ld Philosopher %d is dead\n", now - table->start,
					current->id);
				pthread_mutex_unlock(&table->print_mutex);
				pthread_mutex_unlock(&current->protect);
				set_stop(table, 1);
				return (NULL);
			}
			pthread_mutex_unlock(&current->protect);
			current = current->next;
			if (current == table->head)
				break ;
		}
		if (check_eat(table->head, table->args))
		{
			pthread_mutex_lock(&table->print_mutex);
			printf("All philosophers have eaten enough\n");
			pthread_mutex_unlock(&table->print_mutex);
			set_stop(table, 1);
			return (NULL);
		}
		usleep(1000);
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
	pthread_create(&table->monitor_thread, NULL, monitor, (void *)table);
	pthread_join(table->monitor_thread, NULL);
	i = 0;
	tmp = philo;
	while (i < table->args->nb_philo)
	{
		pthread_join(tmp->thread, NULL);
		tmp = tmp->next;
		i++;
	}
}

t_philo	*manage(int *ac, t_args *args, char **argv, t_table *table)
{
	ft_convert_args(ac, args, argv);
	table->args = args;
	table->stop = 0;
	pthread_mutex_init(&table->print_mutex, NULL);
	pthread_mutex_init(&table->stop_mutex, NULL);
	table->head = init_table(args, table);
	create_thread(table, table->head);
	return (table->head);
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
