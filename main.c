#include <mlx.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

# define WIN_WIDTH 1800 
# define WIN_HEIGHT 1100 
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


// make the color switch
// add a Makefile 

typedef struct s_img {
	void *mlx_img;
	char *addr;
	int bpp;
	int line_len;
	int endian;
} t_img;

typedef struct s_layout {
	double min_i;
	double max_i;
	double min_r;
	double max_r;
} t_layout;

//typedef struct s_color {
//	unsigned char a;
//	unsigned char r;
//	unsigned char g;
//	unsigned char b;
//} t_color;

typedef struct s_fractal {
	int type;
	t_layout layout;
	void *mlx_ptr;
	void *mlx_win;
	float julia_zr;
	float julia_zi;
	t_img img;
} t_fractal;
	
void put_pixel(t_img *img, int x, int y, unsigned int color)
{
	char *pixel = img->addr + (img->line_len * y + x * (img->bpp / 8));
	*(unsigned int *)pixel = color;
}

int mandelbrot(double cr, double ci)
{
	double zr = 0.0;	
	double zi = 0.0;	
	double tmp;
	int n = 0;
	
	while (n < MAX_ITER) {	
		if ((zr * zr + zi * zi) > 4)
			break;
		tmp = zr * zr - zi * zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = tmp;
		n++;
	}
	return (n);
}

int julia(double zr, double zi, double kr, double ki)
{
	double tmp;
	int n = 0;

	while (n < MAX_ITER)
	{	
		if ((zr * zr + zi * zi) > 4)
			break;
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

unsigned int get_color(int n, int MaxIterations) {
	unsigned char r, g, b, a;

	a = 0;
	int halfIterations = MaxIterations / 2;

	if (n < halfIterations) {
		float t = (float)n / (halfIterations - 1);
		g = (unsigned char)(255 * t);
		r = 0;
		b = 0;
	} else {
		float t = (float)(n - halfIterations) / (MaxIterations - halfIterations - 1);
		g = 255;
		r = (unsigned char)(255 * t);
		b = (unsigned char)(255 * t);
	}
	return (unsigned int)((a << 24) | (r << 16) | (g << 8) | b); 
}


void render(t_fractal fractal)
{
	int x = 0;
	int y = 0;
	double cr;
	double ci;
	int n;
	while (x < WIN_WIDTH)
	{	
		cr = fractal.layout.min_r + x * ((fractal.layout.max_r - fractal.layout.min_r) / (WIN_WIDTH - 1));
		while (y < WIN_HEIGHT)
		{
			ci = fractal.layout.max_i - y * ((fractal.layout.max_i - fractal.layout.min_i) / (WIN_HEIGHT - 1));
			if (fractal.type == MANDELBROT)
				n = mandelbrot(cr, ci);
			else if (fractal.type == JULIA)
				n = julia(cr, ci, -0.4, -0.59);
			else if (fractal.type == BURNINGSHIP)
				n = burningship(cr, ci);
			if (n == MAX_ITER)
				put_pixel(&fractal.img, x, y, 0xFF000000);
			else 
				put_pixel(&fractal.img, x, y, get_color(n, MAX_ITER));
			
			y++;
		}
		x++;
		y = 0;
	}
	mlx_put_image_to_window(fractal.mlx_ptr, fractal.mlx_win, fractal.img.mlx_img, 0, 0);
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

void move(t_layout *layout, double distance, int keycode)
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

int mouse_handler(int keycode, int x, int y, t_fractal *fractal)
{
	if (keycode == 4)
	{
		x -= WIN_WIDTH / 2;
		y -= WIN_HEIGHT / 2;
		if (y < 0)
			move(&fractal->layout, (double)y * -1 / WIN_HEIGHT, KEY_UP);
		else if (y > 0)
			move (&fractal->layout, (double)y / WIN_HEIGHT, KEY_DOWN);
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
	//else if (keycode == KEY_SPACE)
		//color_shift(&fractal->layout);
	//else if (!key_event_extend(keycode, &fractal->layout))
		//return (1);
	else
		return (1);
	render(*fractal);
	return (0);
}


int ft_strcmp(char *s1, char *s2)
{
	while(*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
	

void write_usage()
{	
	write (1, "Usage: fractol [OPTION] [JULIA PARAMS]\n\n", 40);
	write (1, "{OPTION}:\n", 10);
	write (1, "	M - Mandelbrot\n", 17);
	write (1, "	J - Julia\n", 12);
	write (1, "	B - Burning Ship\n", 19);
}
	
int set_fractal_type(int ac, char **av, t_fractal *fractal)
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

void set_fractal_layout(t_fractal *fractal)
{
	fractal->layout.min_r = -2.0;
	fractal->layout.max_r = 2.0;
	fractal->layout.min_i = -2.0;
	fractal->layout.max_i = fractal->layout.min_i + (fractal->layout.max_r - fractal->layout.min_r) * WIN_HEIGHT / WIN_WIDTH;
}
	
int set_julia_params(t_fractal *fractal)
{
	// TODO: handle julia params 
	return (0);
}

int main(int ac, char **av)
{
	t_fractal fractal;

	if (set_fractal_type(ac, av, &fractal))
		return (1);
	if (set_julia_params(&fractal))
		return (1);
	set_fractal_layout(&fractal);
	fractal.mlx_ptr = mlx_init();
	if (!fractal.mlx_ptr)
		return (1);
	fractal.mlx_win = mlx_new_window(fractal.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "fract-ol");
	if (!fractal.mlx_win)
	{
		mlx_destroy_display(fractal.mlx_ptr);
		free(fractal.mlx_ptr);
		return (1);
	}
	fractal.img.mlx_img = mlx_new_image(fractal.mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	fractal.img.addr = mlx_get_data_addr(fractal.img.mlx_img, &fractal.img.bpp, &fractal.img.line_len, &fractal.img.endian);
	render(fractal);
	mlx_hook(fractal.mlx_win, CLOSE_WIN, 0, mlx_loop_end, fractal.mlx_ptr);
	mlx_mouse_hook(fractal.mlx_win, mouse_handler, &fractal);
	mlx_key_hook(fractal.mlx_win, key_handler, &fractal);
	mlx_loop(fractal.mlx_ptr);
	mlx_destroy_image(fractal.mlx_ptr, fractal.img.mlx_img);
	mlx_destroy_window(fractal.mlx_ptr, fractal.mlx_win);
	mlx_destroy_display(fractal.mlx_ptr);
	free(fractal.mlx_ptr);
	return (0);
}
