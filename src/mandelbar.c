/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbar.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahorker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/09 02:39:39 by ahorker           #+#    #+#             */
/*   Updated: 2019/08/09 02:39:39 by ahorker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

int				iter_man(t_frac *frc, t_complex c)
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

void			*mandelbar(void *thr)
{
	t_thr		*t;
	t_complex	c;
	int			i;
	int			x;

	t = (t_thr *)thr;
	while (++(t->in) < t->out)
	{
		c.im = (t->frc->max.im + t->frc->p[2] - t->in * t->frc->factor.im);
		x = -1;
		while (++x < SC_Y)
		{
			c.re = (t->frc->min.re + t->frc->p[1] + x * t->frc->factor.re);
			if ((i = iter_man(t->frc, c)) == t->frc->mx_i)
				continue ;
			draw_point(t->frc, x, t->in, i / (double)t->frc->mx_i);
		}
	}
	return (NULL);
}

void			print_usage_error(int x)
{
	if (x == 1)
		write(1, "Malloc error: insufficient memory.\n", 35);
	else
		write(1, "Usage:\n$ ./Fractol Type_of_fractal [color_1 color_2]\n"
				"Note:\n\tType fractal: Mandelbrot, Julia or Mandelbar.\n"
				"\tColor various: red, green or blue.\n", 142);
	exit(x);
}

t_complex		init_complex(double re, double im)
{
	t_complex	x;

	x.im = im;
	x.re = re;
	return (x);
}
