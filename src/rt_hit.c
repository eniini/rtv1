/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_hit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 17:04:50 by eniini            #+#    #+#             */
/*   Updated: 2022/02/25 13:55:02 by eniini           ###   ########.fr       */
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

/*
*	Solves equation for an infinitely long cylinder. Therefore we don't need to
*	calculate intersection points with the shapes' caps.
*/
float	hit_cylinder(t_obj cyl, t_ray ray, float *hit_distance)
{
	t_vector	dist;
	float		a;
	float		b;
	float		c;
	float		d;

	dist = mv_sub_v(ray.orig, cyl.pos);
	a = mv_dot(ray.dir, ray.dir) - powf(mv_dot(ray.dir, cyl.dir), 2);
	b = (mv_dot(dist, ray.dir) - mv_dot(ray.dir, cyl.dir) \
		* mv_dot(dist, cyl.dir)) * 2.0f;
	c = mv_dot(dist, dist) - mv_dot(dist, cyl.dir) * \
	mv_dot(dist, cyl.dir) - cyl.r * cyl.r;
	d = solve_quadratic(a, b, c);
	if (d > 0.0001f && d < *hit_distance)
	{
		*hit_distance = d;
		return (d);
	}
	return (-1.0f);
}

/*
*	Returns a distance to the point of intersection between a ray and
*	an infinite sized cone. [r] exists as a multiplier that denotes the angle
*	cones' extension. 
*/
float	hit_cone(t_obj cone, t_ray ray, float *hit_distance)
{
	t_vector	abc;
	t_vector	dis;
	float		r;
	float		d;

	cone.r = cone.r * DEG_TO_RAD;
	r = (1.0f + cone.r * cone.r);
	dis = mv_sub_v(ray.orig, cone.pos);
	abc.x = mv_dot(ray.dir, ray.dir) - r * powf(mv_dot(ray.dir, cone.dir), 2);
	abc.y = 2 * (mv_dot(dis, ray.dir) - r
			* mv_dot(ray.dir, cone.dir) * mv_dot(dis, cone.dir));
	abc.z = mv_dot(dis, dis) - r * powf(mv_dot(dis, cone.dir), 2);
	d = solve_quadratic(abc.x, abc.y, abc.z);
	if (d > 0.0001f && d < *hit_distance)
	{
		abc = mv_add_v(ray.orig, mv_mul_f(ray.dir, d));
		if ((mv_dot(mv_sub_v(abc, cone.pos), cone.dir) > 0))
			return (-1.0f);
		*hit_distance = d;
		return (d);
	}
	return (-1.0f);
}

/*
*	Returns a distance to the point of intersection between [ray] & [plane] or
*	(-1.0f) if no solution/infinite solutions exist(s).
*
*	Ray hits a plane if the vector from point hit to plane's origin
*	is perpendicular (dotproduct of 0) to the plane's normal (direction) vector.
*	[d] is the distance from the ray's origin to the point hit.
*	Check for very small denominator values avoids cases where there could
*	be infinite or zero hits.
*/
float	hit_plane(t_obj plane, t_ray ray, float *hit_distance)
{
	float		denom;
	float		d;

	denom = mv_dot(ray.dir, plane.dir);
	if (fabs(denom) > 0.0001f)
	{
		d = mv_dot(mv_sub_v(plane.pos, ray.orig), plane.dir) / denom;
		if (d > 0.0001f && d < *hit_distance)
		{
			*hit_distance = d;
			return (d);
		}
	}
	return (-1.0f);
}

/*
*	Returns a distance between the point of intersection between the [ray] & 
*	[sphere] that is closest to ray's point of origin. If not intersection is
*	found, returns (-1.0f).
*/
float	hit_sphere(t_obj sphere, t_ray ray, float *hit_distance)
{
	t_vector	oc;
	float		a;
	float		b;
	float		c;
	float		d;

	oc = mv_sub_v(ray.orig, sphere.pos);
	a = mv_dot(ray.dir, ray.dir);
	b = 2.0f * mv_dot(oc, ray.dir);
	c = mv_dot(oc, oc) - (sphere.r * sphere.r);
	d = solve_quadratic(a, b, c);
	if (d > 0.0001f && d < *hit_distance)
	{
		*hit_distance = d;
		return (d);
	}
	return (-1.0f);
}
