/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiteixei <kiteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 04:35:39 by kiteixei          #+#    #+#             */
/*   Updated: 2025/08/22 23:57:21 by kiteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

long	is_valid_number(char *str)
{
	long	result;
	int		sign;
	int		i;

	free((result = 0, sign = 1, i = 0, NULL));
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!ft_isdigit(str[i]))
		return (0);
	while (ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		if ((result * sign) > INT_MAX || (result * sign) < INT_MIN)
			return (0);
		i++;
	}
	if (str[i] != '\0')
		return (0);
	return (result * sign);
}

void	*call_routine(t_table *table, t_philo *philo)
{
	is_eating(table, philo);
	if (get_stop(table))
		return (NULL);
	is_sleeping(table, philo);
	if (get_stop(table))
		return (NULL);
	ft_usleep(table->args->time_sleep, table);
	if (get_stop(table))
		return (NULL);
	is_thinking(table, philo);
	if (get_stop(table))
		return (NULL);
	return (NULL);
}
