/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahorker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 02:39:39 by ahorker           #+#    #+#             */
/*   Updated: 2019/08/09 02:39:39 by ahorker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

void	apply_zoom(t_frac *e, t_complex mouse, double zoom_factor)
{
	double	interpolation;

	interpolation = 1.0 / zoom_factor;
	e->min.re = mouse.re + (e->min.re - mouse.re) * interpolation;
	e->min.im = mouse.im + (e->min.im - mouse.im) * interpolation;
	e->max.re = mouse.re + (e->max.re - mouse.re) * interpolation;
	e->max.im = mouse.im + (e->max.im - mouse.im) * interpolation;
	e->factor = init_complex((e->max.re - e->min.re) / (SC_X - 1),
			(e->max.im - e->min.im) / (SC_Y - 1));
	e->p[3] = (e->max.re - e->min.re) * 0.025;
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

int		choose_color_helper(char *argv_1)
{
	if (ft_strcmp(argv_1, "blue") == 0)
		return (0);
	else if (ft_strcmp(argv_1, "green") == 0)
		return (8);
	else if (ft_strcmp(argv_1, "red") == 0)
		return (16);
	else
		return (24);
}

void	choose_color(t_frac *frc, char *argv_1, char *argv_2)
{
	frc->col[0] = 24;
	frc->col[1] = 24;
	frc->col[2] = 24;
	if (argv_1 && argv_2)
	{
		if ((ft_strcmp(argv_1, "blue") && ft_strcmp(argv_1, "red") &&
				ft_strcmp(argv_1, "green"))
				|| (ft_strcmp(argv_2, "blue") && ft_strcmp(argv_2, "red") &&
				ft_strcmp(argv_2, "green")))
			print_usage_error(0);
		frc->col[0] = choose_color_helper(argv_1);
		frc->col[2] = choose_color_helper(argv_2);
		if (frc->col[0] + frc->col[2] == 8 && ft_strcmp(argv_1, argv_2))
			frc->col[1] = 16;
		else if (frc->col[0] + frc->col[2] == 24 && ft_strcmp(argv_1, argv_2))
			frc->col[1] = 0;
		else if (ft_strcmp(argv_1, argv_2))
			frc->col[1] = 8;
		else
			frc->col[1] = frc->col[0];
	}
}

int		main(int c, char *v[])
{
	t_frac	frc;

	frc.p[7] = 0;
	frc.f = NULL;
	if (c != 1)
	{
		choose_color(&frc, c > 2 ? v[2] : NULL, c > 3 ? v[3] : NULL);
		frc.p[7] = (ft_strcmp(v[1], "Mandelbrot") == 0) ? 1 : frc.p[7];
		frc.f = (ft_strcmp(v[1], "Mandelbrot") == 0) ? mandelbrot : frc.f;
		frc.p[7] = (ft_strcmp(v[1], "Julia") == 0) ? 2 : frc.p[7];
		frc.f = (ft_strcmp(v[1], "Julia") == 0) ? julia : frc.f;
		frc.p[7] = (ft_strcmp(v[1], "Mandelbar") == 0) ? 3 : frc.p[7];
		frc.f = (ft_strcmp(v[1], "Mandelbar") == 0) ? mandelbar : frc.f;
		if (frc.f == NULL || frc.f == 0)
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
