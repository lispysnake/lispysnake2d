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

/**
 * Opaque Ls2DAnimationComponent implementation
 */
struct Ls2DAnimationComponent {
        Ls2DComponent parent; /*< Parent */
};

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable animation_component_vtable = {
        .obj_name = "Ls2DAnimationComponent",
};

Ls2DComponent *ls2d_animation_component_new()
{
        Ls2DAnimationComponent *self = NULL;

        self = calloc(1, sizeof(struct Ls2DAnimationComponent));
        if (ls_unlikely(!self)) {
                return NULL;
        }

        self = ls2d_object_init((Ls2DObject *)self, &animation_component_vtable);
        self->parent.comp_id = LS2D_COMP_ID_ANIMATION;

        return (Ls2DComponent *)self;
}

Ls2DAnimationComponent *ls2d_animation_component_unref(Ls2DAnimationComponent *self)
{
        return ls2d_object_unref(self);
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
