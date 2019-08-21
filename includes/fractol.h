/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahorker <ahorker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 22:19:38 by ahorker           #+#    #+#             */
/*   Updated: 2019/02/06 19:02:05 by ahorker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "../libft/includes/libft.h"

# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# include <stdio.h>
# include <pthread.h>

# define SC_X		(960)
# define SC_Y		(960)

typedef struct		s_mlx
{
	void			*mlx;
	void			*win;
	void			*img;
	int				bpp;
	int				sl;
	int				en;
	char			*s;
}					t_mlx;

typedef struct		s_vector
{
	double			re;
	double			im;
}					t_complex;

typedef struct		s_frac
{
	t_mlx			mlx;
	t_complex		min;
	t_complex		max;
	t_complex		factor;
	t_complex		k;
	int				coord[3];
	long long int	mx_i;
	int				col[4];
	double			p[9];
	void			*(*f)(void *);
}					t_frac;

typedef struct		s_thr
{
	int				in;
	int				out;
	t_frac			*frc;
}					t_thr;

t_complex			init_complex(double re, double im);

void				draw_point(t_frac *frac, int x, int y, double t);

void				*julia(void *thread);

void				*mandelbrot(void *thread);

void				*mandelbar(void *thread);

void				hook(t_frac *fract);

void				init_frac(t_frac *fract, int p);

void				apply_zoom(t_frac *fract, t_complex mous, double zm_fact);

void				image_create_show(t_frac *fract);

void				print_usage_error(int code_of_error);

#endif
