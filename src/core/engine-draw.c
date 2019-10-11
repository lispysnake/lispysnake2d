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

void ls2d_engine_draw(Ls2DEngine *self, Ls2DFrameInfo *frame)
{
        /* Clear the background */
        SDL_SetRenderDrawColor(self->render, 0, 0, 0, 255);
        SDL_RenderClear(self->render);

        /* Clear old bfufer */
        SDL_SetRenderTarget(self->render, self->buffer);
        SDL_RenderClear(self->render);

        /* Draw the active scene */
        if (ls_likely(self->active_scene != NULL)) {
                ls2d_scene_draw(self->active_scene, frame);
        }

        /* Copy the buffer in */
        SDL_SetRenderTarget(self->render, NULL);
        SDL_RenderCopy(self->render, self->buffer, NULL, NULL);

        SDL_RenderPresent(self->render);
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
