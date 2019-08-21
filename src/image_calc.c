/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_calc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahorker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 23:05:30 by ahorker           #+#    #+#             */
/*   Updated: 2019/08/21 23:05:30 by ahorker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

int		set_color_0(double t, int p1, int p2, int p3)
{
	int		a;
	int		b;
	int		c;

	a = (int)(9 * (1 - t) * pow(t, 3) * 255);
	if (p1 == 24 && p2 == 24 && p3 == 24)
		return ((a << 16) | (a << 8) | (a << 0));
	if (p1 == p2 && p2 == p3)
		return (((a << p1)));
	b = (int)(15 * pow((1 - t), 2) * pow(t, 2) * 255);
	c = (int)(8.5 * pow((1 - t), 3) * t * 255);
	return ((0x0 << 24) | (a << p1) | (b << p2) | (c << p3));
}

void	draw_point(t_frac *frac, int x, int y, double t)
{
	int	ind;
	int	p;

	if (y > SC_Y - 1 || y <= 0 || x > SC_X - 1 || x <= 0)
		return ;
	p = set_color_0(t, frac->col[0], frac->col[1],
			frac->col[2]);
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

void	thread_calc(t_frac *frac, void *(*f)(void *))
{
	int				i;
	int				w;
	t_thr			*t;
	pthread_t		*x;

	i = -1;
	w = SC_Y / frac->p[5];
	if ((x = (pthread_t *)malloc(sizeof(pthread_t) * frac->p[5])) == NULL)
		print_usage_error(1);
	if ((t = (t_thr *)malloc(sizeof(t_thr) * frac->p[5])) == NULL)
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
