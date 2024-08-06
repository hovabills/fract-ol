/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adouiyeh <adouiyeh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 03:07:13 by adouiyeh          #+#    #+#             */
/*   Updated: 2024/08/06 05:06:05 by adouiyeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <math.h>
# include <mlx.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define WIN_WIDTH 1800
# define WIN_HEIGHT 1000
# define MAX_ITER 30

# define MANDELBROT 1
# define JULIA 2
# define BURNINGSHIP 3

# define CLOSE_WIN 17

# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_UP 65362
# define KEY_DOWN 65364
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_PLUS 61
# define KEY_MINUS 45
# define KEY_ESC 65307
# define KEY_SPACE 32

# define RED 111
# define GREEN 222
# define BLUE 333

typedef struct s_img
{
	void			*mlx_img;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
}					t_img;

typedef struct s_layout
{
	double			min_i;
	double			max_i;
	double			min_r;
	double			max_r;
}					t_layout;

typedef struct s_color
{
	unsigned char	a;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}					t_color;

typedef struct s_fractal
{
	int				type;
	t_layout		layout;
	int				rgb;
	void			*mlx_ptr;
	void			*mlx_win;
	float			julia_zr;
	float			julia_zi;
	t_img			img;
}					t_fractal;

int					is_digit(char ch);
float				ft_atof(char *str);
int					check_decimal(char *str);
int					julia_param_valid(char *str);
int					ft_strcmp(char *s1, char *s2);

int					mandelbrot(double cr, double ci);
int					julia(double zr, double zi, double kr, double ki);
int					burningship(double cr, double ci);
void				put_pixel(t_img *img, int x, int y, unsigned int color);
unsigned int		get_color(int n, int MaxIterations, int color);

int					key_handler(int keycode, t_fractal *fractal);
int					mouse_handler(int keycode, int x, int y,
						t_fractal *fractal);

void				write_usage(void);
int					set_fractal_type(int ac, char **av, t_fractal *fractal);
void				set_fractal_default(t_fractal *fractal);
int					set_julia_params(int ac, char **av, t_fractal *fractal);

int					fractal_render(int x, int y, t_fractal fractal);
void				render(t_fractal fractal);
void				zoom(t_layout *layout, double zoom);
void				move(t_layout *layout, double distance, int keycode);
void				change_color(t_fractal *fractal);

void				hook(t_fractal *fractal);
void				clean_exit(t_fractal *fractal);
#endif
