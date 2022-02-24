/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_inits.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 00:53:02 by eniini            #+#    #+#             */
/*   Updated: 2022/02/24 23:33:23 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
*	[0] = White
*	[2] = Black
*	[2] = ???
*	[3] = ???
*	[4] = ???
*	[5] = Violet
*	[6] = Purple
*	[7] = Pink
*	[8] = Ice
*	[9] = Blue
*/
void	init_colors(t_rt *rt)
{
	rt->colors[0] = (t_color){1.0, 1.0, 1.0};
	rt->colors[1] = (t_color){0.0, 0.0, 0.0};
	rt->colors[2] = (t_color){0.62, 0.59, 0.65};
	rt->colors[3] = (t_color){0.70, 0.57, 0.62};
	rt->colors[4] = (t_color){0.54, 0.51, 0.68};
	rt->colors[5] = (t_color){0.48, 0.61, 0.76};
	rt->colors[6] = (t_color){0.49, 0.48, 0.76};
	rt->colors[7] = (t_color){0.62, 0.48, 0.76};
	rt->colors[8] = (t_color){0.48, 0.72, 0.76};
	rt->colors[9] = (t_color){0.48, 0.58, 0.76};
}

t_ray	init_ray(t_cam cam, float u, float v)
{
	t_ray	r;

	r.orig = cam.orig;
	r.dir = cam.llc;
	r.dir = mv_add_v(r.dir, mv_mul_f(cam.h, u));
	r.dir = mv_add_v(r.dir, mv_mul_f(cam.v, v));
	r.dir = mv_sub_v(r.dir, cam.orig);
	//r.dir = mv_normalize(r.dir); //<--------- !!!
	r.normal = (t_vector){0, 0, 0, 0};
	return (r);
}

/*
*	This could be expanded to be more runtime-defineable.
*	Vector [llc] stands for "lower left corner", which points to the starting
*	point in the perspective projection 'screen' that raytracer moves through.
*/
void	init_cam(t_cam *cam, t_vector lookfrom, t_vector lookat)
{
	float	theta;
	float	h;
	float	vfov;
	float	ar;

	vfov = 75;
	ar = 16.0f / 9.0f;
	theta = vfov * DEG_TO_RAD;
	h = tanf(theta / 2.0f);
	cam->orig = lookfrom;
	cam->h = (t_vector){(ar * 2 * h), 0, 0, 1};
	cam->v = (t_vector){0, (2 * h), 0, 1};
	cam->llc = mv_sub_v(mv_sub_v(mv_sub_v(cam->orig, \
				mv_div_f(cam->v, 2.0f)), mv_div_f(cam->h, 2.0f)), \
				mv_normalize(mv_sub_v(lookfrom, lookat)));
	//mv_normalize(cam->llc);
}
