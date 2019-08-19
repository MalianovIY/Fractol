/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahorker <ahorker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 22:19:38 by ahorker           #+#    #+#             */
/*   Updated: 2019/02/06 19:02:05 by ahorker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "../libft/includes/libft.h"

# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# include <stdio.h>
# include <pthread.h>
# define NUM_THRD			(40)
# define SC_X				(960)
# define SC_Y				(960)
# define EC					0.00015230484
# define CO					0.99984769515
# define SI					0.01745240643
# define ABS_MINUS(a, b)	((a >= b) ? (a - b) : (b - a))

typedef struct				s_mlx
{
	void					*mlx;
	void					*win;
	void					*img;
	int						bpp;
	int						sl;
	int						en;
	char					*s;
}							t_mlx;

typedef struct				s_vector
{
	double					re;
	double					im;
}							t_complex;

typedef struct				s_frac
{
	t_mlx					mlx;
	t_complex				min;
	t_complex				max;
	t_complex				factor;
	long long int			mx_i;
	unsigned int			color[2];
	double					p[10];
	void					*(*f)(void *);
}							t_frac;

typedef struct				s_thr
{
	int						in;
	int						out;
	t_frac					*frc;
}							t_thr;

t_complex		init_complex(double re, double im);

#endif
