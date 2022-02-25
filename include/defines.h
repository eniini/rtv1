/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 21:28:13 by eniini            #+#    #+#             */
/*   Updated: 2022/02/25 00:56:48 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# include <stdint.h>

# define WIN_W	800
# define WIN_H	600
# define SPHERE		1
# define CYLINDER	2
# define CONE		3
# define PLANE		4
# define WIN_NAME	"RTv1 build version 0.666"
# define C_BLACK	0xff000000
# define C_WHITE	0xffffffff
# define C_DEBUG	0xff26ff26

# ifndef FLT_MAX
#  define FLT_MAX 	3.402823466e+38F
# endif

typedef struct s_vector {
	float	x;
	float	y;
	float	z;
	float	w;
}			t_vector;
typedef t_vector	t_vertex;

//4 x 4 matrix
typedef struct s_mat4 {
	float	m[4][4];
}			t_mat4;

//Datatype to store an ARBG image data. Each pixel can be found by calculating
//[(h * y) + (x % w)]
typedef struct s_buffer {
	uint32_t	*px;
	uint32_t	w;
	uint32_t	h;
}				t_buffer;

typedef struct s_color {
	float	r;
	float	g;
	float	b;
}			t_color;

#endif
