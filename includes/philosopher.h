/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <kiteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:56:59 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/17 18:39:27 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# ifndef ERROR
#  define ERROR "Paramaters invalid\n"
# endif
# ifndef END_MSG
#  define END_MSG "Le temps est depasser\n"
# endif

typedef struct s_args
{
	int				nb_philo;
	int				time_dead;
	int				time_sleep;
	int				time_eat;
}					t_args;

typedef struct s_philo
{
	int				id;
	struct s_philo	*next;
	int				count_philo;
	int				last_meal;
	int				meals_eaten;
	pthread_mutex_t	*fork_g;
	pthread_mutex_t	*fork_d;
	pthread_t		thread;
	int				i_d;
	pthread_mutex_t	fork;
	struct s_table	*table;
}					t_philo;

typedef struct s_table
{
	t_philo			*head;
	int				count_live_philo;
	int				count_live_eat;
	int				start;
	int				stop;
	int				print_mutex;
	int				stop_mutex;
	int				now;
	t_args			*args;
}					t_table;
#endif
