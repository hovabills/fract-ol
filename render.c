/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adouiyeh <adouiyeh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 03:07:31 by adouiyeh          #+#    #+#             */
/*   Updated: 2024/08/06 05:18:16 by adouiyeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	fractal_render(int x, int y, t_fractal fractal)
{
	double	cr;
	double	ci;

	cr = fractal.layout.min_r + x * ((fractal.layout.max_r
				- fractal.layout.min_r) / (WIN_WIDTH - 1));
	ci = fractal.layout.max_i - y * ((fractal.layout.max_i
				- fractal.layout.min_i) / (WIN_HEIGHT - 1));
	if (fractal.type == MANDELBROT)
		return (mandelbrot(cr, ci));
	if (fractal.type == JULIA)
		return (julia(cr, ci, fractal.julia_zr, fractal.julia_zi));
	if (fractal.type == BURNINGSHIP)
		return (burningship(cr, ci));
	return (0);
}

void	render(t_fractal fractal)
{
	int	x;
	int	y;
	int	n;

	x = 0;
	y = 0;
	while (x < WIN_WIDTH)
	{
		while (y < WIN_HEIGHT)
		{
			n = fractal_render(x, y, fractal);
			if (n == MAX_ITER)
				put_pixel(&fractal.img, x, y, 0xFF000000);
			else
				put_pixel(&fractal.img, x, y, get_color(n, MAX_ITER,
						fractal.rgb));
			y++;
		}
		x++;
		y = 0;
	}
	mlx_put_image_to_window(fractal.mlx_ptr, fractal.mlx_win,
		fractal.img.mlx_img, 0, 0);
}

void	zoom(t_layout *layout, double zoom)
{
	double	center_r;
	double	center_i;

	center_r = layout->min_r - layout->max_r;
	center_i = layout->max_i - layout->min_i;
	layout->max_r = layout->max_r + (center_r - zoom * center_r) / 2;
	layout->min_r = layout->max_r + zoom * center_r;
	layout->min_i = layout->min_i + (center_i - zoom * center_i) / 2;
	layout->max_i = layout->min_i + zoom * center_i;
}

void	move(t_layout *layout, double distance, int keycode)
{
	double	center_r;
	double	center_i;

	center_r = layout->max_r - layout->min_r;
	center_i = layout->max_i - layout->min_i;
	if (keycode == KEY_UP || keycode == KEY_W)
	{
		layout->min_i += center_i * distance;
		layout->max_i += center_i * distance;
	}
	else if (keycode == KEY_DOWN || keycode == KEY_S)
	{
		layout->min_i -= center_i * distance;
		layout->max_i -= center_i * distance;
	}
	else if (keycode == KEY_RIGHT || keycode == KEY_D)
	{
		layout->min_r += center_r * distance;
		layout->max_r += center_r * distance;
	}
	else if (keycode == KEY_LEFT || keycode == KEY_A)
	{
		layout->min_r -= center_r * distance;
		layout->max_r -= center_r * distance;
	}
}

void	change_color(t_fractal *fractal)
{
	if (fractal->rgb == GREEN)
		fractal->rgb = RED;
	else if (fractal->rgb == RED)
		fractal->rgb = BLUE;
	else if (fractal->rgb == BLUE)
		fractal->rgb = GREEN;
}
