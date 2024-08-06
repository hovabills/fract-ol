/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adouiyeh <adouiyeh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 03:07:35 by adouiyeh          #+#    #+#             */
/*   Updated: 2024/08/06 06:03:27 by adouiyeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	is_digit(char ch)
{
	return (ch >= '0' && ch <= '9');
}

float	ft_atof_decimal(char *str, float n)
{
	float	div;

	div = 10.0f;
	str++;
	while (*str)
	{
		n += (*str - '0') / div;
		div *= 10.0f;
		str++;
	}
	return (n);
}

float	ft_atof(char *str)
{
	float	n;
	float	sign;

	n = 0.0f;
	sign = 1.0f;
	while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\v'
		|| *str == '\f' || *str == '\r')
		str++;
	if (*str == '-')
		sign *= -1.0f;
	if (*str == '-' || *str == '+')
		str++;
	while (*str && *str != '.')
	{
		n = n * 10 + *str - '0';
		str++;
	}
	return (ft_atof_decimal(str, n) * sign);
}

int	check_decimal(char *str)
{
	if (*str++ != '.')
		return (0);
	if (!is_digit(*str))
		return (0);
	while (*str)
		if (!is_digit(*str++))
			return (0);
	if (!*str)
		return (1);
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
