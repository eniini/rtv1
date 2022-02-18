/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mm_transformations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 21:22:50 by eniini            #+#    #+#             */
/*   Updated: 2022/02/19 00:07:06 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_mat4	mm_init_unitmatrix(void)
{
	return ((t_mat4){{
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}}});
}

t_mat4	mm_init_translation(float x, float y, float z)
{
	return ((t_mat4){{
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{x, y, z, 1}}});
}
