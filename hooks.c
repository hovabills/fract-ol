/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adouiyeh <adouiyeh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 03:07:16 by adouiyeh          #+#    #+#             */
/*   Updated: 2024/08/06 05:48:39 by adouiyeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	key_handler(int keycode, t_fractal *fractal)
{
	if (keycode == KEY_ESC)
	{
		mlx_loop_end(fractal->mlx_ptr);
		return (0);
	}
	else if (keycode == KEY_PLUS)
		zoom(&fractal->layout, 0.5);
	else if (keycode == KEY_MINUS)
		zoom(&fractal->layout, 2);
	else if (keycode == KEY_UP || keycode == KEY_W)
		move(&fractal->layout, 0.2, keycode);
	else if (keycode == KEY_DOWN || keycode == KEY_S)
		move(&fractal->layout, 0.2, keycode);
	else if (keycode == KEY_LEFT || keycode == KEY_A)
		move(&fractal->layout, 0.2, keycode);
	else if (keycode == KEY_RIGHT || keycode == KEY_D)
		move(&fractal->layout, 0.2, keycode);
	else if (keycode == KEY_SPACE)
		change_color(fractal);
	else
		return (1);
	render(*fractal);
	return (0);
}

int	mouse_handler(int keycode, int x, int y, t_fractal *fractal)
{
	if (keycode == 4)
	{
		x -= WIN_WIDTH / 2;
		y -= WIN_HEIGHT / 2;
		if (y < 0)
			move(&fractal->layout, (double)y * -1 / WIN_HEIGHT, KEY_UP);
		else if (y > 0)
			move(&fractal->layout, (double)y / WIN_HEIGHT, KEY_DOWN);
		if (x < 0)
			move(&fractal->layout, (double)x * -1 / WIN_WIDTH, KEY_LEFT);
		else if (x > 0)
			move(&fractal->layout, (double)x / WIN_WIDTH, KEY_RIGHT);
		zoom(&(fractal->layout), 0.5);
	}
	if (keycode == 5)
		zoom(&(fractal->layout), 2);
	render(*fractal);
	return (0);
}

void	put_pixel(t_img *img, int x, int y, unsigned int color)
{
	char	*pixel;

	pixel = img->addr + (img->line_len * y + x * (img->bpp / 8));
	*(unsigned int *)pixel = color;
}
