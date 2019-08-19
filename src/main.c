#include "../includes/fractol.h"

//array p[i]
//0	zoom
//1	move_x
//2	move_y
//3	step_x_y
//4	step_zoom
//5 num_of_thr
//5 num_of_thr

void	print_usage()
{
	printf("xxx");
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
	e->p[3] = (e->max.re - e->max.re) * .25;
}

t_complex		init_complex(double re, double im)
{
	t_complex	x;

	x.im = im;
	x.re = re;
	return (x);
}

void	init_frac(t_frac *frac)
{
	frac->mx_i = 40;
	frac->p[0] = 1;
	frac->p[1] = 0;
	frac->p[2] = 0;
	frac->p[3] = 4 * 0.25;
	frac->p[4] = 0.01;
	frac->p[5] = 120;
	frac->p[6] = 1 / (double)frac->mx_i;
	frac->min = init_complex(-2.0 * SC_Y / SC_X, -2.0);
	frac->max.re = 2.0 * SC_Y / SC_X;
	frac->max.im = frac->min.im + (frac->max.re - frac->min.re) * SC_Y / SC_X;
	frac->factor = init_complex((frac->max.re - frac->min.re) / (SC_X - 1),
			(frac->max.im - frac->min.im) / (SC_Y - 1));
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

int		set_color(double t)//, unsigned int
// c_1, unsigned int c_2)
{
	int		red, green, blue;

	red = (int)(9 * (1 - t) * t * t * t * 255);
	green = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
	blue = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
	return ((0x0 << 24) | (red << 16) | (green << 8) | blue);
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
			if ((i = iter_mal(t->frc, c)) + 1 == t->frc->mx_i)
				continue ;
			draw_point(t->frc, x, t->in, set_color(i * t->frc->p[6]));//,
					//t->frc->color[0], t->frc->color[1]));
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
	x = (pthread_t *)malloc(sizeof(pthread_t) * (unsigned int)frac->p[5]);
	t = (t_thr *)malloc(sizeof(t_thr) * (unsigned int)frac->p[5]);
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

void	choose_key(t_frac *frac, int key)
{
//	printf("%d\n", frc->mx_i);
	if (key == 53)
	{
		exit(0);
	}
	if (key == 36)
	{
		;
	}
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
	if (key == 43)
		frac->p[4] -= 0.005;
	if (key == 47)
		frac->p[4] += 0.005;
}

void	image_create_show(t_frac *t)
{
//	ft_bzero(t->mlx.s, sizeof(t->mlx.s));
	thread_calc(t, t->f);
	draw_x(t);
	mlx_clear_window(t->mlx.mlx, t->mlx.win);
	mlx_put_image_to_window(t->mlx.mlx, t->mlx.win, t->mlx.img, 0, 0);
	mlx_destroy_image(t->mlx.mlx, t->mlx.img);
}

int		deal_mouse(int key, int x, int y, void *param)
{
	t_frac		*t;
	t_complex	mouse;

	t = (t_frac *)param;

	printf("%d\n", key);
	y = SC_Y - y;
	if (key == 4)
	{
		mouse = init_complex((double)x / (SC_X / (t->max.re -
				t->min.re)) + t->min.re, (double)y / (SC_Y / (t->max.im -
				t->min.im)) + t->min.im);
		apply_zoom(t, mouse, 1 + t->p[4]);
	}
	if (key == 5)
	{
		if (t->min.im == -2.0 && t->min.re == -2.0 && t->max.im == 2.0 &&
				t->max.re == 2.0)
			return (0);
		mouse = init_complex((double)x / (SC_X / (t->max.re -
				t->min.re)) + t->min.re, (double)y / (SC_Y / (t->max.im -
				t->min.im)) + t->min.im);
		apply_zoom(t, mouse, 1 / (double) (1 + t->p[4]));
	}
	t->mlx.img = mlx_new_image(t->mlx.mlx, SC_X, SC_Y);
	t->mlx.s = mlx_get_data_addr(t->mlx.img, &(t->mlx.bpp),
			&(t->mlx.sl), &(t->mlx.en));
	image_create_show(t);
	return (0);
}

int		deal_key(int key, void *param)
{
	t_frac	*t;

	t = (t_frac *)param;
	choose_key(t, key);
	t->mlx.img = mlx_new_image(t->mlx.mlx, SC_X, SC_Y);
	t->mlx.s = mlx_get_data_addr(t->mlx.img, &(t->mlx.bpp),
			&(t->mlx.sl), &(t->mlx.en));
	image_create_show(t);
	return (0);
}

int		main(int argc, char *argv[])
{
	t_frac	frc;

	if (argc != 1)
	{
		frc.color[0] = argc > 3 ? strtol(argv[2], NULL, 0) : 0xFFFFFF;
		frc.color[1] = argc > 4 ? strtol(argv[3], NULL, 0) : frc.color[0];
		frc.mlx.mlx = mlx_init();
		frc.mlx.win = mlx_new_window(frc.mlx.mlx, SC_X, SC_Y, "Fractol");
		frc.mlx.img = mlx_new_image(frc.mlx.mlx, SC_X, SC_Y);
		frc.mlx.s = mlx_get_data_addr(frc.mlx.img,
				&(frc.mlx.bpp), &(frc.mlx.sl), &(frc.mlx.en));
		init_frac(&frc);
		if (ft_strcmp(argv[1], "Mandelbrot") == 0)
			frc.f = mandelbrot;
		image_create_show(&frc);
		mlx_hook(frc.mlx.win, 2, 5, deal_key, &frc);
		mlx_hook(frc.mlx.win, 4,  5, deal_mouse, &frc);
		mlx_hook(frc.mlx.win, 17, 0, close, NULL);
		mlx_loop(frc.mlx.mlx);
	}
	else
		print_usage();
	return (0);
}