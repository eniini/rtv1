/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:38:49 by eniini            #+#    #+#             */
/*   Updated: 2022/02/21 01:28:13 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*static t_bool	is_in_shadow(t_rt *rt, t_ray ray, uint obj_i)
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
}*/

static t_vector	find_intersection_normal(t_obj obj, t_ray ray)
{
	t_vector	hp;
	t_vector	tmp;

	hp = mv_add_v(ray.orig, mv_mul_f(ray.dir, ray.hit_dist));
	if (obj.shape == PLANE)
		return (mv_normalize(mv_mul_f(obj.dir, -1.0f)));
	if (obj.shape == SPHERE)
		return (mv_normalize(mv_sub_v(hp, obj.pos)));
	if (obj.shape == CYLINDER)
	{
		tmp = mv_sub_v(hp, obj.pos);
		tmp = mv_add_v(obj.pos, mv_mul_f(obj.dir, \
				mv_dot(tmp, mv_normalize(obj.dir))));
		return (mv_normalize(mv_sub_v(hp, tmp)));
	}
	else
	{
		ray.normal = mv_sub_v(hp, obj.pos);
		tmp = mv_mul_f(ray.normal, mv_dot(obj.dir, ray.normal));
		ray.normal = mv_div_f(tmp, mv_dot(ray.normal, ray.normal));
		return (mv_normalize(mv_sub_v(obj.dir, ray.normal)));
	}
}

/*
*	NOTE: CALL SHADOWCASTER FROM HERE!
*/
static t_color	find_color(t_rt *rt, t_ray ray, int i)
{
	float	dotproduct;

	ray.normal = find_intersection_normal(rt->object[i], ray);
	dotproduct = mv_dot(ray.normal, mv_normalize(rt->light.pos));
	if (dotproduct > 0)
		return (col_multiply(rt->object[i].col, dotproduct));
	else
		return (rt->colors[1]);
}

/*
*	Note: returns the hit_distance values simply because there was no room
*	to include a boolean return check!
*/
static float	trace_ray(t_rt *rt, t_ray ray, float u, float v)
{
	float		t;
	uint		i;
	t_color		pixelcolor;

	ray.hit_dist = FLT_MAX;
	i = 0;
	pixelcolor = (t_color){0, 0, 0};
	while (i < rt->objcount)
	{
		if (rt->object[i].shape == PLANE)
			t = hit_plane(rt->object[i], ray, &ray.hit_dist);
		if (rt->object[i].shape == SPHERE)
			t = hit_sphere(rt->object[i], ray, &ray.hit_dist);
		if (rt->object[i].shape == CYLINDER)
			t = hit_cylinder(rt->object[i], ray, &ray.hit_dist);
		if (rt->object[i].shape == CONE)
			t = hit_cone(rt->object[i], ray, &ray.hit_dist);
		if (t > 0)
			pixelcolor = find_color(rt, ray, i);
		draw_pixel((uint32_t)(u * WIN_W), (uint32_t)(v * WIN_H), \
			rt->rend.rt_buffer, col_to_uint(pixelcolor));
		i++;
	}
	return (ray.hit_dist);
}

/*
*	Map screen coordinates into UV coordinates (0.0,1.0)
*	going top-to-bottom, left-to-right.
*/
void	render(t_rt *rt)
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
			if (trace_ray(rt, init_ray(rt->cam, u, v), u, v) == FLT_MAX)
				draw_pixel((uint32_t)(u * WIN_W), (uint32_t)(v * WIN_H), \
				rt->rend.rt_buffer, C_BLACK);
			i++;
		}
		j--;
	}
}
