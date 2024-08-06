/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adouiyeh <adouiyeh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 03:07:21 by adouiyeh          #+#    #+#             */
/*   Updated: 2024/08/06 04:58:19 by adouiyeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	clean_exit(t_fractal *fractal)
{
	mlx_destroy_image(fractal->mlx_ptr, fractal->img.mlx_img);
	mlx_destroy_window(fractal->mlx_ptr, fractal->mlx_win);
	mlx_destroy_display(fractal->mlx_ptr);
	free(fractal->mlx_ptr);
}

void	hook(t_fractal *fractal)
{
	mlx_hook(fractal->mlx_win, CLOSE_WIN, 0, mlx_loop_end, fractal->mlx_ptr);
	mlx_mouse_hook(fractal->mlx_win, mouse_handler, fractal);
	mlx_key_hook(fractal->mlx_win, key_handler, fractal);
}

int	main(int ac, char **av)
{
	t_fractal	fractal;

	if (set_fractal_type(ac, av, &fractal) || set_julia_params(ac, av,
			&fractal))
		return (1);
	set_fractal_default(&fractal);
	fractal.mlx_ptr = mlx_init();
	if (!fractal.mlx_ptr)
		return (1);
	fractal.mlx_win = mlx_new_window(fractal.mlx_ptr, WIN_WIDTH, WIN_HEIGHT,
			"fract-ol");
	if (!fractal.mlx_win)
	{
		mlx_destroy_display(fractal.mlx_ptr);
		free(fractal.mlx_ptr);
		return (1);
	}
	fractal.img.mlx_img = mlx_new_image(fractal.mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	fractal.img.addr = mlx_get_data_addr(fractal.img.mlx_img, &fractal.img.bpp,
			&fractal.img.line_len, &fractal.img.endian);
	render(fractal);
	hook(&fractal);
	mlx_loop(fractal.mlx_ptr);
	clean_exit(&fractal);
	return (0);
}
