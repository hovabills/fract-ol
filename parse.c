/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adouiyeh <adouiyeh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 03:07:27 by adouiyeh          #+#    #+#             */
/*   Updated: 2024/08/06 06:02:10 by adouiyeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	write_usage(void)
{
	write(1, "Usage: fractol [OPTION] [JULIA PARAMS]\n\n", 40);
	write(1, "{OPTION}:\n", 10);
	write(1, "	M - Mandelbrot\n", 17);
	write(1, "	J - Julia\n", 12);
	write(1, "	B - Burning Ship\n", 19);
}

int	set_fractal_type(int ac, char **av, t_fractal *fractal)
{
	if (ac < 2)
		return (write_usage(), 1);
	av++;
	if (!ft_strcmp(*av, "M"))
	{
		fractal->type = MANDELBROT;
		return (0);
	}
	if (!ft_strcmp(*av, "J"))
	{
		fractal->type = JULIA;
		return (0);
	}
	if (!ft_strcmp(*av, "B"))
	{
		fractal->type = BURNINGSHIP;
		return (0);
	}
	return (write_usage(), 1);
}

void	set_fractal_default(t_fractal *fractal)
{
	fractal->rgb = GREEN;
	fractal->layout.min_r = -2.0;
	fractal->layout.max_r = 2.0;
	fractal->layout.min_i = -2.0;
	fractal->layout.max_i = fractal->layout.min_i + (fractal->layout.max_r
			- fractal->layout.min_r) * WIN_HEIGHT / WIN_WIDTH;
}

int	julia_param_valid(char *str)
{
	int	n;

	n = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\v'
		|| *str == '\f' || *str == '\r')
		str++;
	if (*str == '+' || *str == '-')
		str++;
	while (*str && *str != '.')
	{
		if (!is_digit(*str))
			return (0);
		n = n * 10 + *str - '0';
		if (n >= 3)
			return (0);
		str++;
	}
	return (check_decimal(str));
}

int	set_julia_params(int ac, char **av, t_fractal *fractal)
{
	if (fractal->type != JULIA)
		return (0);
	if (ac == 4)
	{
		if (julia_param_valid(av[2]) && julia_param_valid(av[3]))
		{
			fractal->julia_zr = ft_atof(av[2]);
			fractal->julia_zi = ft_atof(av[3]);
			return (0);
		}
		else
		{
			write(1, "Invalid Julia params\n", 21);
			return (1);
		}
	}
	else if (ac == 2)
	{
		fractal->julia_zr = -0.835f;
		fractal->julia_zi = -0.232f;
		return (0);
	}
	else
		return (write(1, "Wrong number of Julia params\n", 29));
	return (0);
}
