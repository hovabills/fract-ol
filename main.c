#include <mlx.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define WIN_WIDTH 1000 
#define WIN_HEIGHT 720 
#define MAX_ITER 30 

#define MANDELBROT 1
#define JULIA 2

#define CLOSE_WIN 17 

#define UP 111
#define DOWN 222 
#define RIGHT 333
#define LEFT 444 

// step 1: Create a function that iterates over every pixel represented as "c" and calculates if it belongs to the fractal set
// step 2: Color the pixel with black if it belongs to the set and color the ot

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

typedef struct s_color {
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
} t_color;

typedef struct s_fractal {
	int type;
	t_layout layout;
	void *mlx_ptr;
	void *mlx_win;
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
	
unsigned int get_color(int n, int MaxIterations) {
	unsigned char r, g, b, a;

	a = 0;
	int halfIterations = MaxIterations / 2;

	if (n < halfIterations) {
		float t = (float)n / (halfIterations - 1);
		r = (unsigned char)(255 * t);
		g = 0;
		b = 0;
	} else {
		float t = (float)(n - halfIterations) / (MaxIterations - halfIterations - 1);
		r = 255;
		g = (unsigned char)(255 * t);
		b = (unsigned char)(255 * t);
	}
	return (unsigned int)((a << 24) | (r << 16) | (g << 8) | b);
}

void render(t_img *img, t_layout layout, int fractal)
{
	int x = 0;
	int y = 0;
	double cr;
	double ci;
	int n;
	while (x < WIN_WIDTH)
	{	
		cr = layout.min_r + x * ((layout.max_r - layout.min_r) / (WIN_WIDTH - 1));
		while (y < WIN_HEIGHT)
		{
			ci = layout.max_i - y * ((layout.max_i - layout.min_i) / (WIN_HEIGHT - 1));
			if (fractal == MANDELBROT)
				n = mandelbrot(cr, ci);
			else if (fractal == JULIA)
				n = julia(cr, ci, -0.8, 0.156);
			if (n == MAX_ITER)
				put_pixel(img, x, y, 0xFF000000);
			else 
				put_pixel(img, x, y, get_color(n, MAX_ITER));
			
			y++;
		}
		x++;
		y = 0;
	}
}

void render2(t_fractal fractal)
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
				n = julia(cr, ci, 0.285, 0.01);
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

void move(t_layout *layout, double distance, int direction)
{
	double	center_r;
	double	center_i;

	center_r = layout->max_r - layout->min_r;
	center_i = layout->max_i - layout->min_i;
	if (direction == UP)
	{
		layout->min_i += center_i * distance;
		layout->max_i += center_i * distance;
	}
	else if (direction == DOWN)
	{
		layout->min_i -= center_i * distance;
		layout->max_i -= center_i * distance;
	}
	else if (direction == RIGHT)
	{
		layout->min_r += center_r * distance;
		layout->max_r += center_r * distance;
	}
	else if (direction == LEFT)
	{
		layout->min_r -= center_r * distance;
		layout->max_r -= center_r * distance;
	}
}
	
	

int mouse_handler(int keycode, int x, int y, t_fractal *fractal)
{
	if (keycode == 4)
		zoom(&(fractal->layout), 0.5);
	if (keycode == 5)
		zoom(&(fractal->layout), 2);
	printf("zoom %lf\n", fractal->layout.max_r);
	render2(*fractal);
	return (0);
}
int main()
{

	t_fractal fractal;

	fractal.type = JULIA; 
	fractal.layout.min_r = -2.0;
	fractal.layout.max_r = 2.0;
	fractal.layout.min_i = -2.0;
	fractal.layout.max_i = fractal.layout.min_i + (fractal.layout.max_r - fractal.layout.min_r) * WIN_HEIGHT / WIN_WIDTH;

	fractal.mlx_ptr = mlx_init();
	if (!fractal.mlx_ptr)
		return (0);
	fractal.mlx_win = mlx_new_window(fractal.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "fract-ol");
	if (!fractal.mlx_win)
	{
		mlx_destroy_display(fractal.mlx_ptr);
		free(fractal.mlx_ptr);
		return (1);
	}
	fractal.img.mlx_img = mlx_new_image(fractal.mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	fractal.img.addr = mlx_get_data_addr(fractal.img.mlx_img, &fractal.img.bpp, &fractal.img.line_len, &fractal.img.endian);
	//render(&fractal.img, fractal.layout, fractal.type);
	render2(fractal);
	mlx_hook(fractal.mlx_win, CLOSE_WIN, 0, mlx_loop_end, fractal.mlx_ptr);
	mlx_mouse_hook(fractal.mlx_win, mouse_handler, &fractal);
	mlx_
	mlx_loop(fractal.mlx_ptr);
	mlx_destroy_image(fractal.mlx_ptr, fractal.img.mlx_img);
	mlx_destroy_window(fractal.mlx_ptr, fractal.mlx_win);
	mlx_destroy_display(fractal.mlx_ptr);
	free(fractal.mlx_ptr);
}
