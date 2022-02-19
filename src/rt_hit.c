/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_hit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 17:04:50 by eniini            #+#    #+#             */
/*   Updated: 2022/02/19 20:03:45 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
*	As long as our program doesn't require both intersection points, we can
*	simply return the one that is the closest.
*/
static float	solve_quadratic(float a, float b, float c)
{
	float	discriminant;
	float	t0;
	float	t1;

	discriminant = (b * b) - (4 * a * c);
	if (discriminant < 0)
		return (-1.0f);
	discriminant = sqrtf(discriminant);
	t0 = (-b + discriminant) / 2.0f;
	t1 = (-b - discriminant) / 2.0f;
	if (t0 > t1)
		t0 = t1;
	if (t0 > 0.0001f)
		return (t0);
	else
		return (-1.0f);
}

float	hit_cylinder(t_obj cyl, t_ray ray)
{
	t_vector	dist;
	float		a;
	float		b;
	float		c;

	dist = mv_sub_v(ray.orig, cyl.pos);
	a = mv_dot_product(ray.dir, ray.dir) - \
	mv_dot_product(ray.dir, cyl.dir) * mv_dot_product(ray.dir, cyl.dir);
	b = (mv_dot_product(dist, ray.dir) - mv_dot_product(ray.dir, cyl.dir) \
		* mv_dot_product(dist, cyl.dir)) * 2.0f;
	c = mv_dot_product(dist, dist) - mv_dot_product(dist, cyl.dir) * \
	mv_dot_product(dist, cyl.dir) - cyl.r * cyl.r;
	return (solve_quadratic(a, b, c));
}

float	hit_cone(t_obj cone, t_ray ray)
{
	t_vector	abd;
	t_vector	abc;

	/*
	t_vector	d;
	t_vector	abc;
	float		radius;
	float		quad;
	t_vector	hitpoint;

	radius = (1 + cone.r * cone.r);
	d = mv_sub_v(ray.orig, cone.pos);
	abc.x = mv_dot_product(ray.dir, ray.dir) - radius * \
	mv_dot_product(ray.dir, cone.dir) * mv_dot_product(ray.dir, cone.dir);
	abc.y = (mv_dot_product(d, ray.dir) - radius * \
	mv_dot_product(ray.dir, cone.dir) * mv_dot_product(d, cone.dir) * 2.0f);
	abc.z = mv_dot_product(d, d) - radius * \
	mv_dot_product(d, cone.dir) * mv_dot_product(d, cone.dir) - cone.r * cone.r;
	quad = solve_quadratic(abc.x, abc.y, abc.z);
	if (quad > 0.001f)
	{
		hitpoint = mv_add_v(ray.orig, mv_mul_f(ray.dir, quad));
		if (mv_dot_product(mv_sub_v(hitpoint, cone.pos), cone.dir) > 0)
			return (-1.0f);
		return (quad);
	}
	return (-1.0f);*/
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

	denom = mv_dot_product(ray.dir, plane.dir);
	if (fabs(denom) > 0.0001f)
	{
		t = mv_dot_product(mv_sub_v(plane.pos, ray.orig), plane.dir) / denom;
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

	oc = mv_sub_v(ray.orig, sphere.pos);
	a = mv_dot_product(ray.dir, ray.dir);
	b = 2.0f * mv_dot_product(oc, ray.dir);
	c = mv_dot_product(oc, oc) - (sphere.r * sphere.r);
	return (solve_quadratic(a, b, c));
}
