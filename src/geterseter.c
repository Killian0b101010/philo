/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geterseter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <kiteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:55:57 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/22 01:08:12 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

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
