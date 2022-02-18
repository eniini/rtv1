/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 21:28:19 by eniini            #+#    #+#             */
/*   Updated: 2022/02/19 00:11:50 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

//c lib
# include <string.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
//SDL
# include "../libSDL2/include/SDL2/SDL.h"
//libft
# include "../libft/includes/libft.h"
# include "../libft/includes/ft_gfx.h"
//rt
# include "defines.h"
# include "m_utils.h"

/*
*	shapes:
*		1 - sphere
*		2 - cylinder
*		3 - cone
*		4 - plane
*/
typedef struct s_obj {
	int			shape;
	t_vector	pos;
	t_vector	dir;
	float		r;
	t_color		col;
}				t_obj;

typedef struct s_light {
	t_vector	pos;
	t_color		col;
}				t_light;

typedef struct s_cam {
	float		view_h;
	float		view_w;
	float		focal_l;
	t_vector	orig;
	t_vector	h;
	t_vector	v;
	t_vector	llc;
}				t_cam;

typedef struct s_ray {
	t_vector	orig;
	t_vector	dir;
	t_vector	hit_point;
	t_vector	hp_normal;
	float		hit_dist;
}				t_ray;

//Holds everything related directly to SDL's drawbuffer.
typedef struct s_rend
{
	SDL_Renderer	*renderer;
	SDL_Window		*win;
	SDL_Texture		*win_tex;
	void			*win_pixels;
	t_buffer		*win_buffer;
	t_buffer		*rt_buffer;
	int				win_pixel_pitch;
	t_bool			run;
}					t_rend;

//superstruct that holds all the subsystem structs and the object / light lists.
typedef struct s_rt {
	t_rend		rend;
	t_cam		cam;
	t_obj		*object;
	uint		objcount;
	t_light		light;
	uint		lightcount;
	t_color		colors[10];
}				t_rt;

uint32_t	col_to_uint(t_color color);
t_color		col_lerp(t_color c1, t_color c2, float p);
t_color		col_blend(t_color base, t_color mix, float p);
t_color		col_multiply(t_color color, float m);

void		draw_pixel(uint32_t x, uint32_t y, t_buffer *buf, uint32_t color);

void		rt_render(t_rt *rt);
void		rt_init_cam(t_cam *cam, t_vector lookfrom, t_vector lookat);

float		hit_plane(t_obj plane, t_ray ray);
float		hit_sphere(t_obj sphere, t_ray ray);

void		init_light(t_light *light, t_vector pos, t_color c);
void		init_sphere(t_obj *obj, t_vector orig, float radius, t_color c);
void		init_plane(t_obj *obj, t_vector orig, t_vector dir, t_color c);

void		read_file(t_rt *rt, char *argv);

void		apply_3d_mods(t_obj *obj, t_vector r, t_vector t);

#endif
