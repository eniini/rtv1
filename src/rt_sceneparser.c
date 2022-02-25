/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_sceneparser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:48:00 by eniini            #+#    #+#             */
/*   Updated: 2022/02/25 13:39:45 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
*	As with the camera struct, currently program only handles one light source.
*	Therefore this func simply redefines its position / color etc.
*/
static void	init_l(t_rt *rt, char *line)
{
	char		*ptr;
	t_vector	pos;

	ptr = (line + 2);
	pos = read_3dvec(ptr);
	init_light(&rt->light, pos, rt->colors[0]);
}

/*
*	Assumes being able to read two sets of 3d vector values followed by
*	optional value for object radius. Naive execution runs with all results,
*	even ones resulting from non-numerical input.
*	Note: automatically normalizes given direction vectors at this stage.
*/
static void	init_object(t_rt *rt, char *ptr, char c, int obj_n)
{
	t_vector	pos;
	t_vector	dir;
	float		r;
	t_color		color;

	color = rt->colors[(obj_n % 8) + 2];
	pos = read_3dvec(ptr);
		ptr = ft_strchr(ptr, ' ');
	if (!ptr)
		ft_getout("Invalid object init! (Incorrect delimiter character)");
	dir = mv_normalize(read_3dvec(ptr + 1));
	ptr = ft_strchr(ptr + 1, ' ');
	if (!ptr)
		ft_getout("Invalid object init! (Incorrect delimiter character)");
	r = ft_atof(ptr + 1);
	if (c == 's')
		init_sphere(&rt->object[obj_n], pos, r, color);
	else
		dir.w = r;
	if (c == 'p')
		init_plane(&rt->object[obj_n], pos, dir, color);
	if (c == 'o')
		init_cylinder(&rt->object[obj_n], pos, dir, color);
	if (c == 'v')
		init_cone(&rt->object[obj_n], pos, dir, color);
}

/*
*	First value read points to the object that the modifier is applied to.
*	Second set of values is converted into a 3d vector that's used to apply
*	rotation/transformation modifier.
*	Note: rotation vector is then read as degrees.
*/
static void	init_mod(t_rt *rt, char *line)
{
	int			obj_n;
	char		*ptr;
	t_vector	rot_values;
	t_vector	tra_values;

	rot_values = (t_vector){0, 0, 0, 1};
	tra_values = (t_vector){0, 0, 0, 1};
	ptr = (line + 2);
	if (*line == 't' || *line == 'r')
	{
		if (!(ft_isdigit((int)*ptr)))
			ft_getout("Invalid modifier init! (Non-numerical object id)");
		obj_n = ft_atoi(ptr);
		ptr = ft_strchr(ptr, ' ');
		if (!ptr)
			ft_getout("Invalid modifier init! (Incorrect delimiter character)");
		if (*line == 't')
			tra_values = read_3dvec(ptr + 1);
		else
			rot_values = read_3dvec(ptr + 1);
	}
	else
		ft_getout("Invalid modifier type declaration!");
	apply_3d_mods(&rt->object[obj_n], rot_values, tra_values);
}

/*
*	Process control. Check main function for [values] explanation.
*	Light initialization has to be its own func since object inits
*	count towards the value thats read beforehand (not dynamically allocated).
*/
static void	read_file_cont(t_rt *rt, char *line, t_vector *values)
{
	if (!(ft_strcmp(line, "")))
			values->y++;
	else if ((ft_strncmp(line, "**", 2)))
	{
		if ((int)values->y == 0)
			read_cam(rt, line);
		else if ((int)values->y == 1)
			read_objcount(rt, line);
		else if ((int)values->y == 2 && line[0] == 'l')
			init_l(rt, line);
		else if ((int)values->y == 2 && (uint)values->z < rt->objcount)
		{
			if (line[0] == 's' || line[0] == 'p' \
				|| line[0] == 'v' || line[0] == 'o')
			{
				init_object(rt, (line + 2), line[0], (int)values->z++);
			}
			else
				ft_getout("Invalid object type declaration!");
		}
		else if ((int)values->y == 3)
			init_mod(rt, line);
	}
	free(line);
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
		read_file_cont(rt, line, &values);
	if (close((int)values.x))
		ft_getout(strerror(errno));
}
