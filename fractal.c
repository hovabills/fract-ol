/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractal.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adouiyeh <adouiyeh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 03:07:07 by adouiyeh          #+#    #+#             */
/*   Updated: 2024/08/06 05:46:45 by adouiyeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	mandelbrot(double cr, double ci)
{
	double	zr;
	double	zi;
	double	tmp;
	int		n;

	zr = 0.0;
	zi = 0.0;
	n = 0;
	while (n < MAX_ITER)
	{
		if ((zr * zr + zi * zi) > 4)
			break ;
		tmp = zr * zr - zi * zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = tmp;
		n++;
	}
	return (n);
}

int	julia(double zr, double zi, double kr, double ki)
{
	double	tmp;
	int		n;

	n = 0;
	while (n < MAX_ITER)
	{
		if ((zr * zr + zi * zi) > 4)
			break ;
		tmp = zr * zr - zi * zi + kr;
		zi = 2.0 * zr * zi + ki;
		zr = tmp;
		n++;
	}
	return (n);
}

int	burningship(double cr, double ci)
{
	int		n;
	double	zr;
	double	zi;
	double	tmp;

	zr = 0;
	zi = 0;
	n = 0;
	while (n < MAX_ITER)
	{
		if ((zr * zr + zi * zi) > 4.0)
			break ;
		zr = fabs(zr);
		zi = fabs(zi) * -1;
		tmp = 2 * zr * zi + ci;
		zr = zr * zr - zi * zi + cr;
		zi = tmp;
		n++;
	}
	return (n);
}

unsigned int	rgb_shift(t_color color, int rgb)
{
	if (rgb == RED)
		return ((color.a << 24) | (color.r << 16) | (color.g << 8) | color.b);
	else if (rgb == GREEN)
		return ((color.a << 24) | (color.g << 16) | (color.r << 8) | color.b);
	else
		return ((color.a << 24) | (color.b << 16) | (color.g << 8) | color.r);
}

unsigned int	get_color(int n, int MaxIterations, int rgb)
{
	t_color	color;
	float	t;
	int		half_iter;

	color.a = 0;
	half_iter = MaxIterations / 2;
	if (n < half_iter)
	{
		t = (float)n / (half_iter - 1);
		color.r = (unsigned char)(255 * t);
		color.g = 0;
		color.b = 0;
	}
	else
	{
		t = (float)(n - half_iter) / (MaxIterations - half_iter - 1);
		color.r = 255;
		color.g = (unsigned char)(255 * t);
		color.b = (unsigned char)(255 * t);
	}
	return (rgb_shift(color, rgb));
}
