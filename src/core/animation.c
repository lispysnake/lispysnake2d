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

#include "ls2d.h"

static void ls2d_animation_destroy(Ls2DAnimation *self);

struct Ls2DAnimation {
        Ls2DObject parent;

        /* Dynamic storage of frames */
        LsArray *frames;
};

/**
 * Animation frame is used internally so we can quickly cycle
 * animation frames and retain a linear cache.
 */
typedef struct Ls2DAnimationFrame {
        uint32_t duration;
        Ls2DTextureHandle texture;
} Ls2DAnimationFrame;

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable animation_vtable = {
        .destroy = (ls2d_object_vfunc_destroy)ls2d_animation_destroy,
        .obj_name = "Ls2DAnimation",
};

Ls2DAnimation *ls2d_animation_new()
{
        Ls2DAnimation *self = NULL;

        self = calloc(1, sizeof(struct Ls2DAnimation));
        if (ls_unlikely(!self)) {
                return NULL;
        }
        self->frames = ls_array_new_size(sizeof(struct Ls2DAnimationFrame), 3);
        if (ls_unlikely(!self->frames)) {
                free(self);
                return NULL;
        }

        return ls2d_object_init((Ls2DObject *)self, &animation_vtable);
}

Ls2DAnimation *ls2d_animation_unref(Ls2DAnimation *self)
{
        return ls2d_object_unref(self);
}

static void ls2d_animation_destroy(Ls2DAnimation *self)
{
        ls_array_free(self->frames, NULL);
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
