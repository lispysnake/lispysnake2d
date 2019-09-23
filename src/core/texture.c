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
#include <stdlib.h>

#include "libls.h"
#include "texture.h"

static void ls2d_texture_destroy(Ls2DTexture *self);

/**
 * Opaque Ls2DTexture implementation
 */
struct Ls2DTexture {
        Ls2DObject object; /*< Parent */

        SDL_Rect dimensions;
        SDL_Texture *texture;
        const char *filename;
};

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable texture_vtable = {
        .destroy = (ls2d_object_vfunc_destroy)ls2d_texture_destroy,
        .obj_name = "Ls2DTexture",
};

Ls2DTexture *ls2d_texture_new_from_filename(const char *filename)
{
        Ls2DTexture *self = NULL;

        self = calloc(1, sizeof(struct Ls2DTexture));
        if (ls_unlikely(!self)) {
                return NULL;
        }
        self->filename = filename;

        return ls2d_object_init((Ls2DObject *)self, &texture_vtable);
}

void ls2d_texture_draw(Ls2DTexture *self, Ls2DFrameInfo *frame, SDL_Point position)
{
        if (ls_unlikely(!self) || ls_unlikely(!self->texture)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ls2DTexture not correctly initialised");
                return;
        }
}

Ls2DTexture *ls2d_texture_unref(Ls2DTexture *self)
{
        return ls2d_object_unref(self);
}

static void ls2d_texture_destroy(Ls2DTexture *self)
{
        if (self->texture) {
                SDL_DestroyTexture(self->texture);
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
