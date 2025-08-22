/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <kiteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:56:59 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/22 20:34:06 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# ifndef ERROR
#  define ERROR "Error : Parameters invalid\n"
# endif
# ifndef ERROR_VALUE
#  define ERROR_VALUE "Error\n"
# endif

typedef struct s_args
{
	int				nb_philo;
	int				time_dead;
	int				time_sleep;
	int				time_eat;
	int				nb_eats;

}					t_args;
typedef struct s_philo
{
	int				id;
	struct s_philo	*next;
	int				last_meal;
	int				meals_eaten;
	pthread_mutex_t	*fork_g;
	pthread_mutex_t	*fork_d;
	pthread_mutex_t	protect;
	pthread_t		thread;
	pthread_mutex_t	fork;
	struct s_table	*table;
}					t_philo;

typedef struct s_table
{
	t_philo			*head;
	unsigned long	start;
	int				stop;
	pthread_t		monitor_thread;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	t_args			*args;
}					t_table;

int					ft_convert_args(int *ac, t_args *args, char **argv);
void				*monitor(void *arg);
t_philo				*manage(int *ac, t_args *args, char **argv, t_table *table);
t_philo				*create_multiple_philo(t_table *table);
t_philo				*init_table(t_args *args, t_table *table);
void				call_one_philo(t_philo *philo, t_table *table);
void				create_thread(t_table *table, t_philo *philo);
void				one_philo(t_philo *philo, t_table *table);
void				*safe_malloc(size_t size);
void				free_philo_no_circle(t_philo *philo);
void				free_philo_circle(t_philo *philo, t_args *args,
						t_table *table);
long				is_valid_number(char *str);
int					get_stop(t_table *table);
void				set_stop(t_table *table, int value);
int					take_fork(t_table *table, t_philo *philo);
void				is_eating(t_table *table, t_philo *philo);
void				*routine(void *arg);
void				*is_sleeping(t_table *table, t_philo *philo);
void				*is_thinking(t_table *table, t_philo *philo);
void				t(int nb);
void				*philo_is_dead(t_philo *current, t_table *table, int now);
void				init_data(t_philo *philo, t_table *table);
t_philo				*create_one_philo(t_table *table);
int					check_eat(t_philo *head, t_args *args);
int					what_time_is_it(void);
int					check_time(t_philo *philo, t_args *args);
int					ft_usleep(int time, t_table *table);
int					time_now(t_table *table);
int				take_fork_two(t_table *table, t_philo *philo);

#endif