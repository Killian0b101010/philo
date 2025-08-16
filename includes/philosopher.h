/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <kiteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:56:59 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/15 01:26:23 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>\

typedef struct s_args
{
	int				nb_philo;
	int				time_dead;
	int				time_sleep;
  int         time_eat;
}					t_args;

typedef struct s_philo
{
	int				id;
	struct s_philo	*next;
	int				count_philo;
  pthread_t thread;
	pthread_mutex_t	fork;
}					t_philo;


#endif
