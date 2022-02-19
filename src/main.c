/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 21:22:33 by eniini            #+#    #+#             */
/*   Updated: 2022/02/19 13:30:11 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
*	Todo:	read up on windowflags in case we could have additional features
*			Q:do we need sethint() and should it be to linear or nearest?
*			same thing with rendermodes, pixelformat etc.
*			A:Currently we don't use any SDL texture transformation funcs so its
*			probably not needed!
*			blending is only needed if we work with multiple texture layers
*			(and if we want to blend them together)
*/
static void	init(t_rt *rt)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		ft_getout(SDL_GetError());
	rt->rend.win = SDL_CreateWindow(WIN_NAME, SDL_WINDOWPOS_UNDEFINED, \
		SDL_WINDOWPOS_UNDEFINED, WIN_W, WIN_H, 0);
	if (!rt->rend.win)
		ft_getout(SDL_GetError());
	rt->rend.renderer = SDL_CreateRenderer(rt->rend.win, -1, \
		SDL_RENDERER_ACCELERATED);
	if (!rt->rend.renderer)
		ft_getout(SDL_GetError());
	rt->rend.win_tex = SDL_CreateTexture(rt->rend.renderer, \
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIN_W, WIN_H);
	if (!rt->rend.win_tex)
		ft_getout(SDL_GetError());
	rt->rend.run = TRUE;
	rt->objcount = 0;
	init_colors(rt);
}

static void	cleanup(t_rt *rt)
{
	SDL_DestroyTexture(rt->rend.win_tex);
	SDL_DestroyRenderer(rt->rend.renderer);
	SDL_DestroyWindow(rt->rend.win);
	free(rt->rend.win_buffer->px);
	free(rt->rend.win_buffer);
	free(rt->object);
	SDL_Quit();
}

/*
*	Note about SDL_LockTexture: void **px is 'filled in', meaning that SDL
*	creates its own allocated pixel buffer thats returned to the given pointer.
*	This is why you need to query for the pixel_pitch too since its the only
*	way to know the 1-directional pitch of the created buffer.
*/
static void	loop(t_rt	*rt)
{
	SDL_Event	e;

	ft_bzero(rt->rend.win_buffer->px, WIN_H * WIN_W * sizeof(uint32_t));
	SDL_PollEvent(&e);
	if (e.window.event == SDL_WINDOWEVENT_CLOSE)
		rt->rend.run = FALSE;
	ft_memcpy(rt->rend.win_buffer->px, rt->rend.rt_buffer->px,
		WIN_H * WIN_W * sizeof(uint32_t));
	if (SDL_LockTexture(rt->rend.win_tex, NULL, \
		&rt->rend.win_pixels, &rt->rend.win_pixel_pitch) < 0)
		ft_getout(SDL_GetError());
	ft_memcpy(rt->rend.win_pixels, rt->rend.win_buffer->px, \
	WIN_H * rt->rend.win_pixel_pitch);
	SDL_UnlockTexture(rt->rend.win_tex);
	if (SDL_RenderCopy(rt->rend.renderer, rt->rend.win_tex, NULL, NULL) < 0)
		ft_getout(SDL_GetError());
	SDL_RenderPresent(rt->rend.renderer);
}

static void	print_usage(void)
{
	ft_putendl("Usage: rtv1 <scene>\n");
	ft_putendl("Check scene_example for formatting guidelines\n");
	exit(EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_rt		rt;

	if (argc != 2)
		print_usage();
	ft_bzero(&rt.rend, sizeof(t_rend));
	rt.rend.win_buffer = (t_buffer *)malloc(sizeof(t_buffer));
	rt.rend.rt_buffer = (t_buffer *)malloc(sizeof(t_buffer));
	if (!rt.rend.win_buffer || !rt.rend.rt_buffer)
		ft_getout("failed to initialize pixel buffers");
	rt.rend.win_buffer->w = WIN_W;
	rt.rend.win_buffer->h = WIN_H;
	rt.rend.win_buffer->px = ft_memalloc(sizeof(uint32_t) * WIN_H * WIN_W);
	rt.rend.rt_buffer->w = WIN_W;
	rt.rend.rt_buffer->h = WIN_H;
	rt.rend.rt_buffer->px = ft_memalloc(sizeof(uint32_t) * WIN_H * WIN_W);
	if (!rt.rend.win_buffer->px || !rt.rend.rt_buffer->px)
		ft_getout("failed to initialize pixel buffers");
	init(&rt);
	read_file(&rt, argv[1]);
	rt_render(&rt);
	while (rt.rend.run)
		loop(&rt);
	cleanup(&rt);
	return (0);
}
