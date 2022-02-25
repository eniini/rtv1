/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_inits.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 00:53:02 by eniini            #+#    #+#             */
/*   Updated: 2022/02/25 14:02:41 by eniini           ###   ########.fr       */
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
	r.dir = mv_normalize(r.dir);
	r.normal = (t_vector){0, 0, 0, 0};
	return (r);
}

static void	init_cam_extend(t_cam *cam, t_vector camdir, float ar, float h)
{
	t_vector	w;
	t_vector	u;
	t_vector	v;
	t_vector	vup;

	vup = (t_vector){0, 1, 0, 1};
	w = mv_normalize(camdir);
	if (w.x == 0 && w.y == -1 && w.z == 0)
		vup = (t_vector){0, 0, -1, 1};
	u = mv_normalize(mv_cross(vup, w));
	v = mv_cross(w, u);
	cam->h = mv_mul_f(u, ar * 2.0f * h);
	cam->v = mv_mul_f(v, 2.0f * h);
	cam->llc = mv_sub_v(cam->orig, mv_div_f(cam->h, 2.0f));
	cam->llc = mv_sub_v(cam->llc, mv_div_f(cam->v, 2.0f));
	cam->llc = mv_sub_v(cam->llc, w);
}

/*
*	This could be expanded to be more runtime-defineable.
*	Vector [llc] stands for "lower left corner", which points to the starting
*	point in the perspective projection 'screen' that raytracer moves through.
*/
void	init_cam(t_cam *cam, t_vector lookfrom, t_vector lookat)
{
	float		theta;
	float		h;
	float		vfov;
	float		ar;

	vfov = 75;
	ar = 4.0f / 3.0f;
	theta = vfov * DEG_TO_RAD;
	h = tanf(theta / 2.0f);
	cam->orig = lookfrom;
	init_cam_extend(cam, mv_sub_v(lookfrom, lookat), ar, h);
}
