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

#include <stdlib.h>

#include "libls.h"
#include "object.h"
#include "sprite.h"

/**
 * Opaque Ls2DSprite implementation
 */
struct Ls2DSprite {
        Ls2DObject object; /*< Parent */
        SDL_Rect area;
};

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable sprite_vtable = {
        .destroy = NULL,
        .obj_name = "Ls2DSprite",
};

Ls2DSprite *ls2d_sprite_new()
{
        Ls2DSprite *self = NULL;

        self = calloc(1, sizeof(struct Ls2DSprite));
        if (ls_unlikely(!self)) {
                return NULL;
        }
        self->area = (SDL_Rect){ 50, 50, 100, 100 };

        return ls2d_object_init((Ls2DObject *)self, &sprite_vtable);
}

Ls2DSprite *ls2d_sprite_unref(Ls2DSprite *self)
{
        return ls2d_object_unref(self);
}

/**
 * Perform the actual sprite drawing. Long story short, we need to
 * draw to our X, Y coordinates if within clip using our set
 * texture.
 */
void ls2d_sprite_draw(Ls2DSprite *self, Ls2DFrameInfo *frame)
{
        SDL_SetRenderDrawColor(frame->renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(frame->renderer, &self->area);
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
