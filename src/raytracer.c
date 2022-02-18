/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:38:49 by eniini            #+#    #+#             */
/*   Updated: 2022/02/19 00:20:51 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_ray	init_ray(t_cam cam, float u, float v)
{
	t_ray	r;

	r.orig = cam.orig;
	r.dir = cam.llc;
	r.dir = mv_add_v(r.dir, mv_mul_f(cam.h, u));
	r.dir = mv_add_v(r.dir, mv_mul_f(cam.v, v));
	r.dir = mv_sub_v(r.dir, cam.orig);
	r.dir = mv_normalize(r.dir);
	r.hit_point = (t_vector){0, 0, 0, 1};
	r.hp_normal = (t_vector){0, 0, 0, 1};
	return (r);
}

static t_bool	is_in_shadow(t_rt *rt, t_ray ray, uint obj_i)
{
	uint	i;
	t_ray	shadowray;
	t_obj	lightobj;

	i = 0;
	while (i < rt->objcount)
	{
		if (rt->object[i].shape == SPHERE && i != obj_i)
		{
			shadowray.orig = ray.hit_point;
			shadowray.dir = mv_normalize(rt->light.pos);
			lightobj.pos = rt->light.pos;
			lightobj.r = 0.001;
			if (hit_sphere(lightobj, shadowray) > 0.001f)
				return (TRUE);
		}
		//other shape checks here
		i++;
	}
	return (FALSE);
}

static t_bool	trace_ray(t_rt *rt, t_ray ray, float u, float v)
{
	float		t;
	uint		i;
	t_color		pixelcolor;

	//ft_printf("light pos: %f|%f|%f\n", rt->light.pos.x, rt->light.pos.y, rt->light.pos.z);
	ray.hit_dist = FLT_MAX;
	i = 0;
	pixelcolor = (t_color){0, 0, 0};
	while (i < rt->objcount)
	{
		if (rt->object[i].shape == PLANE)
		{
			t = hit_plane(rt->object[i], ray);
			if (t > 0.001f && t < ray.hit_dist)
			{
				ray.hit_dist = t;
				ray.hit_point = mv_normalize(mv_add_v(ray.orig, mv_mul_f(ray.dir, ray.hit_dist)));
				ray.hp_normal = mv_mul_f(rt->object[i].dir, -1.0f);
				pixelcolor = rt->object[i].col;
			}
		}
		if (rt->object[i].shape == SPHERE)
		{
			t = hit_sphere(rt->object[i], ray);
			if (t > 0.001f && t < ray.hit_dist)
			{
				ray.hit_dist = t;
				ray.hit_point = mv_add_v(ray.orig, mv_mul_f(ray.dir, ray.hit_dist));
				ray.hp_normal = mv_normalize(mv_sub_v(ray.hit_point, rt->object[i].pos)); //plane doesnt handle like this!
				float dotproduct = mv_dot_product(ray.hp_normal, mv_normalize(rt->light.pos));
				if (dotproduct > 0)
					pixelcolor = col_multiply(rt->object[i].col, dotproduct);
			}
		}
		draw_pixel((uint32_t)(u * WIN_W), (uint32_t)(v * WIN_H), \
			rt->rend.rt_buffer, col_to_uint(pixelcolor));
		i++;
	}
	if (ray.hit_dist != FLT_MAX)
		return (TRUE);
	return (FALSE);
}

void	rt_init_cam(t_cam *cam, t_vector lookfrom, t_vector lookat)
{
	float	theta;
	float	h;
	float	vfov;
	float	ar;

	vfov = 75;
	ar = 16.0f / 9.0f;
	theta = vfov * DEG_TO_RAD;
	h = tanf(theta / 2.0f);
	cam->view_h = 2 * h;
	cam->view_w = ar * cam->view_h;
	cam->orig = lookfrom;
	cam->h = (t_vector){cam->view_w, 0, 0, 1};
	cam->v = (t_vector){0, cam->view_h, 0, 1};
	cam->llc = mv_sub_v(mv_sub_v(mv_sub_v(cam->orig, \
				mv_div_f(cam->v, 2.0f)), mv_div_f(cam->h, 2.0f)), \
				mv_normalize(mv_sub_v(lookfrom, lookat)));
	mv_normalize(cam->llc);
}

/*
*	Map screen coordinates into UV coordinates (0.0,1.0)
*	going top-to-bottom, left-to-right.
*/
void	rt_render(t_rt *rt)
{
	int		i;
	int		j;
	float	u;
	float	v;

	j = WIN_H;
	while (j >= 0)
	{
		i = 0;
		while (i < WIN_W)
		{
			u = (float)i / (WIN_W - 1);
			v = (float)j / (WIN_H - 1);
			if (!(trace_ray(rt, init_ray(rt->cam, u, v), u, v)))
				draw_pixel((uint32_t)(u * WIN_W), (uint32_t)(v * WIN_H), \
				rt->rend.rt_buffer, C_BLACK);
			i++;
		}
		j--;
	}
}
