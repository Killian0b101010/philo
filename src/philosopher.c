/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:42:19 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/14 19:56:19 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"
/*
void ft_convert_args(t_args *args)
{
  args->nb_philo = ft_atoi;
  args->time_dead =ft_atoi;
  args->time_sleep = ft_atoi;
}

void init_args(t_args *args)
{  
  argv[2][0] = args ->nb_philo;
  if(args->nb_philo <= 0)
      return;
  argv[3][0] = args ->time_dead;
  argv[4][0] = args -> time_sleep;
}

void create_thread(t_args *args)
{
  int i = 0;
  while(i < args -> nb_philo)
  {
    
  }
}
int	main(int ac, char **argv)
{
  t_args args;
  if(ac < 3)
    return((write(2,"Error\n.",6),1);
  ft_convert_args(args);
  init_args(args);


}
*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *routine1(void *arg) 
{
  int counter1 = 0;
 
    while (1) {
        counter1++;
        printf("Thread 1: counter1 = %d\n", counter1);
        usleep(100000);
    }
    (void)arg;
    return NULL;
}

void *routine2(void *arg) 
{
  int counter2 = 0;
  (void)arg;
    while (1) {
        counter2++;
        printf("Thread 2: counter2 = %d\n", counter2);
        usleep(100000); 
    }
    
    (void) arg;
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, routine1, NULL);
    pthread_create(&t2, NULL, routine2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
