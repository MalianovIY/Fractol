#include "../includes/fractol.h"

//array p[i]
//0	zoom
//1	move_x
//2	move_y
//3	step_x_y
//4	step_zoom
//5 num_of_thr
//6 1 / max_i
//7 type of fractal
//8 move_paused

void	print_usage_error(int x)
{
	if (x == 1)
		write(1, "Malloc error: insufficient memory.\n", 35);
	else
		write(1, "Usage:\n$ ./Fractol Type_of_fractol [color_1 color_2]\n"
				"Note:\n\tType fractal: Mandelbrot, Julia or Mandelbar.\n"
				"\tColor various: red, green or blue.\n", 142);
	exit(x);
}

double interpolate(double start, double end, double interpolation)
{
	return (start + ((end - start) * interpolation));
}

void apply_zoom(t_frac* e, t_complex mouse, double zoomFactor)
{
	double	interpolation;

	interpolation = 1.0 / zoomFactor;
	e->min.re = interpolate(mouse.re, e->min.re, interpolation);
	e->min.im = interpolate(mouse.im, e->min.im, interpolation);
	e->max.re = interpolate(mouse.re, e->max.re, interpolation);
	e->max.im = interpolate(mouse.im, e->max.im, interpolation);
	e->factor = init_complex((e->max.re - e->min.re) / (SC_X - 1),
			(e->max.im - e->min.im) / (SC_Y - 1));
	e->p[3] = (e->max.re - e->min.re) * .025;
}

t_complex		init_complex(double re, double im)
{
	t_complex	x;

	x.im = im;
	x.re = re;
	return (x);
}

void	init_frac(t_frac *frac, int p)
{
	if (p == 0)
	{
		frac->mlx.mlx = mlx_init();
		frac->mlx.win = mlx_new_window(frac->mlx.mlx, SC_X, SC_Y, "Fractol");
	}
	frac->mx_i = 40;
	frac->p[0] = 1;
	frac->p[1] = 0;
	frac->p[2] = 0;
	frac->p[3] = 4 * 0.25;
	frac->p[4] = 1;
	frac->p[5] = 120;
	frac->p[6] = 1 / (double)frac->mx_i;
	frac->p[8] = 0;
	frac->min = init_complex(-2.0 * SC_Y / SC_X, -2.0);
	frac->max.re = 2.0 * SC_Y / SC_X;
	frac->max.im = frac->min.im + (frac->max.re - frac->min.re) * SC_Y / SC_X;
	frac->factor = init_complex((frac->max.re - frac->min.re) / (SC_X - 1),
			(frac->max.im - frac->min.im) / (SC_Y - 1));
	ft_bzero(frac->coord, sizeof(int) * 3);
}

void	draw_point(t_frac *frac, int x, int y, int p)
{
	int ind;

	if (y > SC_Y - 1 || y <= 0 || x > SC_X - 1 || x <= 0)
		return ;
	ind = 4 * x + frac->mlx.sl * 1 * y;
	frac->mlx.s[ind] = (char)(p & 0xFF);
	frac->mlx.s[ind + 1] = (char)(p >> 8 & 0xFF);
	frac->mlx.s[ind + 2] = (char)(p >> 16 & 0xFF);
}

void	draw_x(t_frac *frac)
{
	int	i;

	i = -1;
	while (++i < (SC_Y >> 6))
	{
		draw_point(frac, i + (SC_Y >> 6) + SC_X / 2, (SC_X >> 1), 0xFFFFFF);
		draw_point(frac, (SC_X >> 1), i + (SC_Y >> 6) + SC_Y / 2, 0xFFFFFF);
		draw_point(frac, -i - (SC_Y >> 6) + SC_X / 2, (SC_X >> 1), 0xFFFFFF);
		draw_point(frac, (SC_X >> 1), -i - (SC_Y >> 6) + SC_Y / 2, 0xFFFFFF);
	}
}

int		set_color_0(double t, int p1, int p2, int p3)
{
	int		a, b, c;

	a = (int)(9 * (1 - t) * pow(t, 3) * 255);
	if (p1 == 24 && p2 == 24 && p3 == 24)
		return ((0x0 << 24) | (a << 16) | (a << 8) | (a << 0));
	b = (int)(15 * pow((1 - t), 2) * pow(t, 2) * 255);
	c = (int)(8.5 * pow((1 - t), 3) * t * 255);
	return ((0x0 << 24) | (a << p1) | (b << p2) | (c << p3));
}

int		iter_man(t_frac *frc, t_complex c)
{
	int			i;
	t_complex	z;

	z = init_complex(c.re, c.im);
	i = -1;
	while (z.re * z.re + z.im * z.im <= 4 && ++i < frc->mx_i)
		z = init_complex(z.re * z.re - z.im * z.im + c.re, -2.0 * z.re
				* z.im + c.im);
	return (i);
}

int		iter_mal(t_frac *frc, t_complex c)
{
	int			i;
	t_complex	z;

	z = init_complex(c.re, c.im);
	i = -1;
	while (z.re * z.re + z.im * z.im <= 4 && ++i < frc->mx_i)
		z = init_complex(z.re * z.re - z.im * z.im + c.re, 2.0 * z.re
				* z.im + c.im);
	return (i);
}

int		iter_jul(t_frac *frc, t_complex c)
{
	int			i;
	t_complex	z;

	z = c;
	i = -1;
	while (z.re * z.re + z.im * z.im <= 4 && ++i < frc->mx_i)
		z = init_complex(z.re * z.re - z.im * z.im + frc->k.re,
				2.0 * z.re * z.im + frc->k.im);
	return (i);
}

void	*julia(void *thr)
{
	t_thr		*t;
	int			i, x;
	t_complex	c;

	t = (t_thr *)thr;
	while (++(t->in) < t->out)
	{
		c.im = (t->frc->max.im + t->frc->p[2] - t->in * t->frc->factor.im)
				;
		x = -1;
		while (++x < SC_Y)
		{
			c.re = (t->frc->min.re + t->frc->p[1] + x * t->frc->factor.re)
					;
 			if ((i = iter_jul(t->frc, c)) == t->frc->mx_i)
				continue ;
			draw_point(t->frc, x, t->in, set_color_0(i / (double)t->frc->mx_i,
					t->frc->color[0], t->frc->color[1], t->frc->color[2]));
		}
	}
	return (NULL);
}

void	*mandelbrot(void *thr)
{
	t_thr		*t;
	t_complex	c;
	int			i, x;

	t = (t_thr *)thr;
	while (++(t->in) < t->out)
	{
		c.im = (t->frc->max.im + t->frc->p[2] - t->in * t->frc->factor.im)
				;
		x = -1;
		while (++x < SC_Y)
		{
			c.re = (t->frc->min.re + t->frc->p[1] + x * t->frc->factor.re)
					;
			if ((i = iter_mal(t->frc, c)) == t->frc->mx_i)
				continue ;
			draw_point(t->frc, x, t->in, set_color_0(i / (double)t->frc->mx_i,
					t->frc->color[0], t->frc->color[1], t->frc->color[2]));
		}
	}
	return (NULL);
}

void	*mandelbar(void *thr)
{
	t_thr		*t;
	t_complex	c;
	int			i, x;

	t = (t_thr *)thr;
	while (++(t->in) < t->out)
	{
		c.im = (t->frc->max.im + t->frc->p[2] - t->in * t->frc->factor.im)
				;
		x = -1;
		while (++x < SC_Y)
		{
			c.re = (t->frc->min.re + t->frc->p[1] + x * t->frc->factor.re)
					;
			if ((i = iter_man(t->frc, c)) == t->frc->mx_i)
				continue ;
			draw_point(t->frc, x, t->in, set_color_0(i / (double)t->frc->mx_i,
					t->frc->color[0], t->frc->color[1], t->frc->color[2]));
		}
	}
	return (NULL);
}

void	thread_calc(t_frac *frac, void *(*f)(void *))
{
	int				i, w;
	t_thr			*t;
	pthread_t		*x;

	i = -1;
	w = SC_Y / frac->p[5];
	if ((x = (pthread_t *)malloc(
			sizeof(pthread_t) * (unsigned int)frac->p[5])) == NULL)
		print_usage_error(1);
	if ((t = (t_thr *)malloc(sizeof(t_thr) * (unsigned int)frac->p[5])) == NULL)
	{
		free(x);
		print_usage_error(1);
	}
	while (++i < (long long int)frac->p[5])
	{
		t[i].in = i * w - 1;
		t[i].out = (i + 1) * w;
		t[i].frc = frac;
		pthread_create(&(x[i]), NULL, f, &t[i]);
	}
	while (i >= 0)
		pthread_join(x[i--], NULL);
	free(x);
	free(t);
}

void	image_create_show(t_frac *t)
{
	t->mlx.img = mlx_new_image(t->mlx.mlx, SC_X, SC_Y);
	t->mlx.s = mlx_get_data_addr(t->mlx.img, &(t->mlx.bpp),
			&(t->mlx.sl), &(t->mlx.en));
	thread_calc(t, t->f);
	draw_x(t);
	mlx_clear_window(t->mlx.mlx, t->mlx.win);
	mlx_put_image_to_window(t->mlx.mlx, t->mlx.win, t->mlx.img, 0, 0);
	mlx_destroy_image(t->mlx.mlx, t->mlx.img);
}

int  mouse_move (int x, int y, void *param)
{
	t_frac *f;

	f = (t_frac *)param;
	if (f->p[7] == 1 || f->p[8] == 1)
		return (0);
	if (x < 0 || x > SC_X)
		x = (x > SC_X) ? SC_X : 0;
	if (y < 0 || y > SC_Y)
		y = (y > SC_Y) ? SC_Y : 0;
	if (x == f->coord[0] || y == f->coord[1])
		return (0);
	f->k = init_complex(4 * ((double)x - SC_X * 0.5) / SC_X,
			4 * ((double)(SC_Y - y) - SC_Y * 0.5) / SC_Y);
	image_create_show(f);
	return (0);
}

int		choose_zoom(t_frac *t, int x, int y, int key)
{
	t_complex	mouse;

	if (key == 4)
	{
		t->p[0] *= 1.25;
		mouse = init_complex((double)x / (SC_X / (t->max.re -
				t->min.re)) + t->min.re, (double)y / (SC_Y / (t->max.im -
				t->min.im)) + t->min.im);
		apply_zoom(t, mouse,  1.25);
	}
	if (key == 5)
	{
		t->p[0] *= 0.8;
		if (t->min.im == -2.0 && t->min.re == -2.0 && t->max.im == 2.0 &&
				t->max.re == 2.0)
			return (0);
		mouse = init_complex((double)x / (SC_X / (t->max.re -
				t->min.re)) + t->min.re, (double)y / (SC_Y / (t->max.im -
				t->min.im)) + t->min.im);
		apply_zoom(t, mouse, 0.8);
	}
	return (1);
}

int		deal_mouse(int key, int x, int y, void *param)
{
	t_frac		*t;
	t = (t_frac *)param;

	y = SC_Y - y;
	if (key == 76)
		init_frac(t, 1);
	if (key == 1)
	{
		t->mx_i += 40;
		t->p[6] = 1 / (double)t->mx_i;
	}
	if (key == 2)
	{
		t->mx_i -= 40;
		t->p[6] = 1 / (double)t->mx_i;
	}
	if (key == 3)
	{
		t->mx_i = 40;
		t->p[6] = 1 / (double)t->mx_i;
	}
	if (key == 4 || key == 5)
		if (choose_zoom(t, x, y, key) == 0)
			return (0);
	image_create_show(t);
	return (0);
}

int		deal_key(int key, void *param)
{
	t_frac	*frac;

	frac = (t_frac *)param;
	if (key == 53)
		exit(0);
	if (key == 49)
		frac->p[8] = frac->p[8] == 0 ? 1 : 0;
	if (key == 13)
		frac->p[2] += frac->p[3];
	if (key == 1)
		frac->p[2] -= frac->p[3];
	if (key == 2)
		frac->p[1] += frac->p[3];
	if (key == 0)
		frac->p[1] -= frac->p[3];
	if (key == 69)
		frac->p[3] -= 0.005;
	if (key == 78)
		frac->p[3] += 0.005;
	image_create_show(frac);
	return (0);
}

void	choose_color(t_frac *frc, char *argv_1, char *argv_2)
{
	frc->color[0] = 24;
	frc->color[1] = 24;
	frc->color[2] = 24;

	if (argv_1 && argv_2)
	{
		if (ft_strcmp(argv_1, "blue") == 0)
			frc->color[0] = 8;
		else if (ft_strcmp(argv_1, "green") == 0)
			frc->color[0] = 0;
		else if (ft_strcmp(argv_1, "red") == 0)
			frc->color[0] = 16;
		if (ft_strcmp(argv_2, "blue") == 0)
			frc->color[2] = 8;
		else if (ft_strcmp(argv_2, "green") == 0)
			frc->color[2] = 0;
		else if (ft_strcmp(argv_2, "red") == 0)
			frc->color[2] = 16;

		if ((frc->color[0] == 16 && frc->color[2] == 8)
				|| (frc->color[0] == 8 && frc->color[2] == 16))
			frc->color[1] = 0;
		else if ((frc->color[0] == 0 && frc->color[2] == 8)
				|| (frc->color[0] == 8 && frc->color[2] == 0))
			frc->color[1] = 16;
		else if ((frc->color[0] == 16 && frc->color[2] == 0)
				|| (frc->color[0] == 0 && frc->color[2] == 16))
			frc->color[1] = 8;
	}
}

void	hook(t_frac *frc)
{
	mlx_hook(frc->mlx.win, 2, 5, deal_key, frc);
	mlx_hook(frc->mlx.win, 4,  5, deal_mouse, frc);
	mlx_hook(frc->mlx.win, 17, 0, close, NULL);
	mlx_hook(frc->mlx.win, 6, 0, mouse_move, frc);
}

int		main(int c, char *v[])
{
	t_frac	frc;

	if (c != 1)
	{
		choose_color(&frc, c > 2 ? v[2] : NULL, c > 3 ? v[3] : NULL);
		if (ft_strcmp(v[1], "Mandelbrot") == 0)
		{
			frc.p[7] = 1;
			frc.f = mandelbrot;
		}
		else if (ft_strcmp(v[1], "Julia") == 0)
		{
			frc.p[7] = 2;
			frc.f = julia;
		}
		else if (ft_strcmp(v[1], "Mandelbar") == 0)
		{
			frc.p[7] = 3;
			frc.f = mandelbar;
		}
		else
			print_usage_error(0);
		init_frac(&frc, 0);
		image_create_show(&frc);
		hook(&frc);
		mlx_loop(frc.mlx.mlx);
	}
	else
		print_usage_error(0);
	return (0);
}