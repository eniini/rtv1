/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mv_vectoral.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 21:23:06 by eniini            #+#    #+#             */
/*   Updated: 2022/02/03 15:14:57 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m_utils.h"
#include <math.h>

/*
*	Dot product (a · b) returns a scalar that respresents how much the two
*	vectors are 'alike'. In practice, return value
*	[< 0] means that the angle between two vectors is more than 90°,
*	[0] means that the angle is exactly 90° and
*	[> 0] means that the angle is less than 90°.
*/
float	mv_dot_product(t_vector va, t_vector vb)
{
	return (va.x * vb.x + va.y * vb.y + va.z * vb.z);
}

/*
*	Returns a magnitude of an perpendicular vector to the plane (v1, v2).
*	Can be used to calculate normals, etc.
*/
t_vector	mv_cross_product(t_vector va, t_vector vb)
{
	t_vector	result;

	result.x = va.y * vb.z - va.z * vb.y;
	result.y = va.z * vb.x - va.x * vb.z;
	result.z = va.x * vb.y - va.y * vb.x;
	return (result);
}

/*
*	Calculates the distance between 2d vectors v1 and v2.
*/
float	mv_distance_to_vector(t_vector va, t_vector vb)
{
	return (sqrt(((va.x - vb.x) * (va.x - vb.x)) + \
			((va.y - vb.y) * (va.y - vb.y))));
}

/*
*	Calculates the length or a magnitude of a vector (from origin point).
*	The result is equal to sqrt(dot(v, v)).
*/
float	mv_length(t_vector v)
{
	return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

/*
*	Vector is normalized i.e. turned into an unit vector, by scaling each of
*	its components by its magnitude.
*/
t_vector	mv_normalize(t_vector v)
{
	t_vector	ret;
	float		l;

	l = mv_length(v);
	if (l == 0.0f)
		return (v);
	ret.x = v.x / l;
	ret.y = v.y / l;
	ret.z = v.z / l;
	ret.w = v.w / l;
	return (ret);
}
