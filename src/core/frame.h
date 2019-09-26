/*
 * This file is part of lispysnake2d.
 *
 * Copyright (c) 2019 Lispy Snake, Ltd.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.

 */

#pragma once

#include <SDL.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

/**
 * The Ls2DFrameInfo object is passed to renderer and update cycles
 * to give them information about the current frame pass.
 */
struct Ls2DFrameInfo {
        uint32_t tick_start;    /**<First tick */
        uint32_t ticks;         /**<Current tick count */
        uint32_t prev_ticks;    /**<Previous tick count */
        SDL_Renderer *renderer; /**<Current renderer */
        SDL_Window *window;     /**<Displayed window */
        uint32_t frames[5];
        uint32_t i_frame;
        uint32_t tick_delay;
};

__attribute__((always_inline)) inline void ls2d_frame_info_init(Ls2DFrameInfo *frame)
{
        frame->tick_start = SDL_GetTicks();
        frame->prev_ticks = frame->tick_start - 1;
}

__attribute__((always_inline)) inline void ls2d_frame_info_tick(Ls2DFrameInfo *frame)
{
        frame->ticks = SDL_GetTicks() - frame->tick_start;
        frame->frames[frame->i_frame % 5] = frame->ticks - frame->prev_ticks;
        frame->i_frame++;
}

__attribute__((always_inline)) inline void ls2d_frame_info_stash(Ls2DFrameInfo *frame)
{
        frame->tick_delay = frame->ticks - frame->prev_ticks;
        frame->prev_ticks = frame->ticks;
}

/*
__attribute__((always_inline)) inline void ls2d_frame_info_delay(Ls2DFrameInfo *frame, uint32_t
fps_delay, double fps)
{
}
*/

__attribute__((always_inline)) inline double ls2d_frame_info_get_fps(Ls2DFrameInfo *frame)
{
        uint32_t total = frame->i_frame >= 5 ? 5 : frame->i_frame;
        double fps = 0;

        for (unsigned int i = 0; i < total; i++) {
                fps += frame->frames[i];
        }
        fps /= total;
        return 1000.0 / fps;
}

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 expandtab:
 * :indentSize=8:tabSize=8:noTabs=true:
 */
