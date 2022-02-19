/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 21:21:54 by eniini            #+#    #+#             */
/*   Updated: 2022/02/19 13:30:14 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
*	[0] = White
*	[1] = Light gray
*	[2] = Medium gray
*	[3] = Dark gray
*	[4] = Black
*	[5] = Violet
*	[6] = Purple
*	[7] = Pink
*	[8] = Ice
*	[9] = Blue
*/
void	init_colors(t_rt *rt)
{
	rt->colors[0] = (t_color){1.0, 1.0, 1.0};
	rt->colors[1] = (t_color){0.75, 0.75, 0.75};
	rt->colors[2] = (t_color){0.5, 0.5, 0.5};
	rt->colors[3] = (t_color){0.25, 0.25, 0.25};
	rt->colors[4] = (t_color){0.0, 0.0, 0.0};
	rt->colors[5] = (t_color){0.48, 0.61, 0.76};
	rt->colors[6] = (t_color){0.49, 0.48, 0.76};
	rt->colors[7] = (t_color){0.62, 0.48, 0.76};
	rt->colors[8] = (t_color){0.48, 0.72, 0.76};
	rt->colors[9] = (t_color){0.48, 0.58, 0.76};
}

//convert t_color to ARGB uint for SDL pixel painting
uint32_t	col_to_uint(t_color color)
{
	int	ir;
	int	ig;
	int	ib;

	ir = (int)(255 * ft_clamp_d(color.r, 0.0, 1.0));
	ig = (int)(255 * ft_clamp_d(color.g, 0.0, 1.0));
	ib = (int)(255 * ft_clamp_d(color.b, 0.0, 1.0));
	return (255 << 24 | ir << 16 | ig << 8 | ib);
}

//linear interpolation of two t_color structs.
t_color	col_lerp(t_color c1, t_color c2, float p)
{
	t_color	result;

	if (p < 0.0f)
		p = 0.0f;
	if (p > 1.0f)
		p = 1.0f;
	result.r = ft_d_lerp(c1.r, c2.r, p);
	result.g = ft_d_lerp(c1.g, c2.g, p);
	result.b = ft_d_lerp(c1.b, c2.b, p);
	return (result);
}

//mix/blend two colors together
t_color	col_blend(t_color base, t_color mix, float p)
{
	t_color	result;
	float	pm;

	if (p < 0.0f)
		p = 0.0f;
	if (p > 1.0f)
		p = 1.0f;
	pm = 1.0f - p;
	result.r = pm * base.r + p * mix.r;
	result.g = pm * base.g + p * mix.g;
	result.b = pm * base.b + p * mix.b;
	return (result);
}

t_color	col_multiply(t_color color, float m)
{
	t_color	result;

	result.r = ft_clamp_d(color.r * m, 0.0, 1.0);
	result.g = ft_clamp_d(color.g * m, 0.0, 1.0);
	result.b = ft_clamp_d(color.b * m, 0.0, 1.0);
	return (result);
}
