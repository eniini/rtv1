/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_parser_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 22:40:02 by eniini            #+#    #+#             */
/*   Updated: 2022/02/24 23:29:52 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
*	Read (x,y,z) values from a character string [line].
*	Only floating point values with a period decimal separator delimited with
*	commas are allowed.
*/
t_vector	read_3dvec(char *line)
{
	t_vector	v;
	char		*ptr;

	ptr = line;
	if (!(*ptr >= '0' && *ptr <= '9') && (*ptr != '-'))
		ft_getout("Invalid vector init! (Incorrect symbol)");
	v.x = ft_atof(ptr);
	ptr = ft_strchr(line, ',');
	if (!ptr)
		ft_getout("Invalid vector init! (Missing vector Y value");
	ptr = (ptr + 1);
	if (!(*ptr >= '0' && *ptr <= '9') && (*ptr != '-'))
		ft_getout("Invalid vector init! (Incorrect symbol)");
	v.y = ft_atof(ptr);
	ptr = ft_strchr(ptr, ',');
	if (!ptr)
		ft_getout("Invalid vector init! (Missing vector Z value");
	ptr = (ptr + 1);
	if (!(*ptr >= '0' && *ptr <= '9') && (*ptr != '-'))
		ft_getout("Invalid vector init! (Incorrect symbol)");
	v.z = ft_atof(ptr);
	v.w = 1.0f;
	return (v);
}

/*
*	Note: as long as we are rendering only a single camera, it doesn't need to
*	be initalized. Multiple initalizations should not break the program, only
*	rewrite previous coordinates.
*/
void	read_cam(t_rt *rt, char *line)
{
	char		*ptr;
	t_vector	lookfrom;
	t_vector	lookat;

	ptr = line;
	lookfrom = mv_normalize(read_3dvec(ptr));
	ptr = ft_strchr(line, ' ');
	if (!ptr)
		ft_getout("Invalid camera init! (Incorrect delimiter character)");
	lookat = mv_normalize(read_3dvec((ptr + 1)));
	init_cam(&rt->cam, lookfrom, lookat);
	rt->light.pos = rt->cam.orig;
	rt->light.col = rt->colors[0];
}

void	read_objcount(t_rt *rt, char *line)
{
	rt->objcount = ft_atoi(line);
	ft_printf("objectcount: %u\n", rt->objcount);
	if (!rt->objcount)
		ft_getout("Invalid object count declaration!");
	rt->object = (t_obj *)malloc(sizeof(t_obj) * rt->objcount);
	if (!rt->object)
		ft_getout("Failed to allocate memory for objects.");
}
