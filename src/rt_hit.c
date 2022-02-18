/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_hit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 17:04:50 by eniini            #+#    #+#             */
/*   Updated: 2022/02/19 00:10:00 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static float	hit_cylinder(t_obj cylinder, t_ray ray)
{

}

static float	hit_cone(t_obj cone, t_ray ray)
{
	
}

/*
*	Returns a distance to the point of intersection between [ray] & [plane] or
*	(-1.0f) if no solution/infinite solutions exist(s).
*
*	Ray hits a plane if the vector from point hit to plane's origin
*	is perpendicular (dotproduct of 0) to the plane's normal (direction) vector.
*	[t] is the distance from the ray's origin to the point hit.
*	Check for very small denominator values avoids cases where there could
*	be infinite or zero hits.
*/
float	hit_plane(t_obj plane, t_ray ray)
{
	float		denom;
	float		t;
	t_vector	diff;

	denom = mv_dot_product(mv_normalize(ray.dir), mv_normalize(plane.dir));
	if (denom > 0.0001f)
	{
		diff = mv_sub_v(mv_normalize(plane.pos), mv_normalize(ray.orig));
		t = mv_dot_product(diff, mv_normalize(plane.dir)) / denom;
		if (t > 0.0001f)
			return (t);
	}
	return (-1.0f);
}

/*
*	Returns a distance between the point of intersection between the [ray] & 
*	[sphere] that is closest to ray's point of origin. If not intersection is
*	found, returns (-1.0f).
*/
float	hit_sphere(t_obj sphere, t_ray ray)
{
	t_vector	oc;
	float		a;
	float		b;
	float		c;
	float		discriminant;

	oc = mv_sub_v(ray.orig, sphere.pos);
	a = mv_dot_product(ray.dir, ray.dir);
	b = 2.0f * mv_dot_product(oc, ray.dir);
	c = mv_dot_product(oc, oc) - (sphere.r * sphere.r);
	discriminant = (b * b) - (4 * a * c);
	if (discriminant < 0)
		return (-1.0f);
	float	sqrtdiscr = sqrtf(discriminant);
 	float	t0 = (-b + sqrtdiscr) / 2.0f;
	float	t1 = (-b - sqrtdiscr) / 2.0f;
	if (t0 > t1)
		t0 = t1;
	if (t0 > 0.0001f)
		return (t0);
	else
		return (-1.0f);
	//return ((-b - sqrtf(discriminant)) / (2.0f * a));
}
