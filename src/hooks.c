/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahorker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 22:55:12 by ahorker           #+#    #+#             */
/*   Updated: 2019/08/21 22:55:12 by ahorker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

int		mouse_move (int x, int y, void *param)
{
	t_frac	*f;

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
	t_frac			*t;

	t = (t_frac *)param;
	y = SC_Y - y;
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
	if (key == 76)
		init_frac(frac, 1);
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
	image_create_show(frac);
	return (0);
}

void	hook(t_frac *frc)
{
	mlx_hook(frc->mlx.win, 2, 5, deal_key, frc);
	mlx_hook(frc->mlx.win, 4, 5, deal_mouse, frc);
	mlx_hook(frc->mlx.win, 17, 0, close, NULL);
	mlx_hook(frc->mlx.win, 6, 0, mouse_move, frc);
}
