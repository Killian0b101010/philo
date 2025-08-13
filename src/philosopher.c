/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:27:02 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/13 20:51:58 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/philosopher.h"

void	*routine(void *arg)
{
	(void)arg;
	printf("Test from threads \n");
	return (NULL);
}

int	main(int ac, char **argv)
{
	pthread_t	t1;
	pthread_t	t2;

	(void)argv;
	(void)ac;
	pthread_create(&t1, NULL, &routine, NULL);
	pthread_create(&t2, NULL, &routine, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	return (0);
}
