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

#include <SDL.h>

#include "engine-private.h"
#include "libls.h"

static void ls2d_engine_process_keyboard(Ls2DEngine *self, SDL_KeyboardEvent *event)
{
        switch (event->keysym.sym) {
        case SDLK_f:
                if (event->type == SDL_KEYUP) {
                        ls2d_engine_set_fullscreen(self, !self->fullscreen);
                }
                break;
        case SDLK_q:
                self->running = false;
                break;
        case SDLK_RIGHT:
        case SDLK_LEFT:
        case SDLK_DOWN:
        case SDLK_UP:
                SDL_Log("Pressed an arrow :O");
                break;
        default:
                break;
        }
        return;
}

void ls2d_engine_process_events(Ls2DEngine *self, Ls2DFrameInfo *frame)
{
        SDL_Event event = { 0 };

        ls2d_scene_update(self->active_scene, frame);
        ls2d_input_manager_process(self->input_manager, frame);

        /* Event update */
        while (SDL_PollEvent(&event) != 0) {
                switch (event.type) {
                case SDL_KEYUP:
                case SDL_KEYDOWN:
                        ls2d_engine_process_keyboard(self, &(event.key));
                        break;
                case SDL_QUIT:
                        self->running = false;
                        break;
                default:
                        break;
                }
        }
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
