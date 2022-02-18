/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_sceneparser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:48:00 by eniini            #+#    #+#             */
/*   Updated: 2022/02/19 00:19:01 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
*	Read (x,y,z) values from a character string [line].
*	Only floating point values with a period decimal separator delimited with
*	commas are allowed.
*/
static t_vector	read_3dvec(char *line)
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
static void	init_cam(t_rt *rt, char *line)
{
	char		*ptr;
	t_vector	lookfrom;
	t_vector	lookat;

	ptr = line;
	lookfrom = read_3dvec(ptr);
	ptr = ft_strchr(line, ' ');
	if (!ptr)
		ft_getout("Invalid camera init! (Incorrect delimiter character)");
	lookat = read_3dvec((ptr + 1));
	rt_init_cam(&rt->cam, lookfrom, lookat);
	rt->light.pos = rt->cam.orig;
	rt->light.col = rt->colors[0];
}

static void	init_objcount(t_rt *rt, char *line)
{
	rt->objcount = ft_atoi(line);
	ft_printf("objectcount: %u\n", rt->objcount);
	if (!rt->objcount)
		ft_getout("Invalid object count declaration!");
	rt->object = (t_obj *)malloc(sizeof(t_obj) * rt->objcount);
	if (!rt->object)
		ft_getout("Failed to allocate memory for objects.");
}

static void	init_l(t_rt *rt, char *line)
{
	char		*ptr;
	t_vector	pos;

	ptr = (line + 2);
	pos = read_3dvec(ptr);
	init_light(&rt->light, pos, rt->colors[0]);
}

/*
*	During the first call, expects to find a value corresponding to how many
*	objects will be declared
*	ft_printf("pos:%1.1f|%1.1f|%1.1f\n", rt->object[obj_n].pos.x, rt->object[obj_n].pos.y, rt->object[obj_n].pos.z);
*	ft_printf("dir:%1.1f|%1.1f|%1.1f\n", rt->object[obj_n].dir.x, rt->object[obj_n].dir.y, rt->object[obj_n].dir.z);
*	ft_printf("shape: %d\n", rt->object[obj_n].shape);
*/
static void	init_obj(t_rt *rt, char *line, int obj_n)
{
	char		*ptr;
	t_vector	pos;
	t_vector	dir;
	float		r;

	ptr = (line + 2);
	if (line[0] == 's')
	{
		pos = read_3dvec(ptr);
		ptr = ft_strchr(ptr, ' ');
		if (!ptr)
			ft_getout("Invalid object init! (Incorrect delimiter character)");
		dir = read_3dvec(ptr + 1);
		ptr = ft_strchr(ptr + 1, ' ');
		if (!ptr)
			ft_getout("Invalid object init! (Incorrect delimiter character)");
		r = ft_atof(ptr + 1);
		init_sphere(&rt->object[obj_n], pos, r, rt->colors[(obj_n % 4) + 5]);
	}
	else if (line[0] == 'p')
	{
		pos = read_3dvec(ptr);
		ptr = ft_strchr(ptr, ' ');
		if (!ptr)
			ft_getout("Invalid object init! (Incorrect delimiter character)");
		dir = read_3dvec(ptr + 1);
		init_plane(&rt->object[obj_n], pos, dir, rt->colors[(obj_n % 4) + 5]);
	}
	else if (line[0] == 'v')
	{
		return ;
	}
	else if (line[0] == 'o')
	{
		return ;
	}
	else
		ft_getout("Invalid object type declaration!");
}

static void	init_mod(t_rt *rt, char *line)
{
	int			obj_n;
	char		*ptr;
	t_vector	rot_values;
	t_vector	tra_values;

	rot_values = (t_vector){0, 0, 0, 1};
	tra_values = (t_vector){0, 0, 0, 1};
	ptr = (line + 2);
	if (*line == 't')
	{
		obj_n = ft_atoi(ptr);
		ptr = ft_strchr(ptr, ' ');
		if (!ptr)
			ft_getout("Invalid object init! (Incorrect delimiter character)");
		tra_values = read_3dvec(ptr + 1);
	}
	else if (*line == 'r')
	{
		obj_n = ft_atoi(ptr);
		ptr = ft_strchr(ptr, ' ');
		if (!ptr)
			ft_getout("Invalid object init! (Incorrect delimiter character)");
		rot_values = read_3dvec(ptr + 1);
	}
	else
		ft_getout("Invalid modifier type declaration!");
	apply_3d_mods(&rt->object[obj_n], rot_values, tra_values);
}

/*
*	Empty lines update section counter, which acts as a initalization delimiter
*	where section 0 is reserved for single camera initialization,
*	section 1 handles object initialization and section 2 applies modifications
*	to the initalized objects.
*
*	values.x = (int)file decriptor
*	values.y = section
*	values.z = obj_i;
*/
void	read_file(t_rt *rt, char *argv)
{
	t_vector	values;
	char		*line;

	values = (t_vector){open(argv, O_RDONLY), 0, 0, 0};
	if ((int)values.x < 0)
		ft_getout(strerror(errno));
	while (get_next_line((int)values.x, &line) == 1)
	{
		if (!(ft_strcmp(line, "")))
			values.y++;
		else if ((ft_strncmp(line, "**", 2)))
		{
			if ((int)values.y == 0)
				init_cam(rt, line);
			else if ((int)values.y == 1)
				init_objcount(rt, line);
			else if ((int)values.y == 2 && line[0] == 'l')
				init_l(rt, line);
			else if ((int)values.y == 2 && (uint)values.z < rt->objcount)
				init_obj(rt, line, (int)values.z++);
			else if ((int)values.y == 3)
				init_mod(rt, line);
		}
		free(line);
	}
	if (close((int)values.x))
		ft_getout(strerror(errno));
}
