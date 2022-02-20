/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_shapes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 17:49:44 by eniini            #+#    #+#             */
/*   Updated: 2022/02/20 20:25:45 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_light(t_light *light, t_vector pos, t_color c)
{
	light->pos = pos;
	light->col = c;
}

void	init_sphere(t_obj *obj, t_vector orig, float radius, t_color c)
{
	obj->shape = SPHERE;
	obj->pos = orig;
	obj->dir = orig;
	obj->r = radius;
	obj->col = c;
}

void	init_plane(t_obj *obj, t_vector orig, t_vector dir, t_color c)
{
	obj->shape = PLANE;
	obj->pos = orig;
	obj->dir = mv_normalize(dir);
	obj->r = 0.0f;
	obj->col = c;
}

/*
*	Note: cylinder's radius is given as the [dir.w].
*/
void	init_cylinder(t_obj *obj, t_vector orig, t_vector dir, t_color c)
{
	t_vector	v;

	obj->shape = CYLINDER;
	obj->pos = orig;
	obj->r = dir.w;
	v = (t_vector){dir.x, dir.y, dir.z, 1};
	obj->dir = v;
	obj->col = c;
}

/*
*	Note: cone's radius is given as [dir.w].
*/
void	init_cone(t_obj *obj, t_vector orig, t_vector dir, t_color c)
{
	t_vector	v;

	obj->shape = CONE;
	obj->pos = orig;
	obj->r = dir.w;
	v = (t_vector){dir.x, dir.y, dir.z, 1};
	obj->dir = v;
	obj->col = c;
}
