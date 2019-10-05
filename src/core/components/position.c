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

#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>

#include "ls2d.h"

static void ls2d_position_component_init(Ls2DPositionComponent *self);

/**
 * Opaque Ls2DPositionComponent implementation
 */
struct Ls2DPositionComponent {
        Ls2DComponent parent; /*< Parent */
        SDL_Point pos;
        int pos_z;
};

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable position_vtable = {
        .obj_name = "Ls2DPositionComponent",
        .init = (ls2d_object_vfunc_init)ls2d_position_component_init,
};

Ls2DComponent *ls2d_position_component_new()
{
        return (Ls2DComponent *)LS2D_NEW(Ls2DPositionComponent, position_vtable);
}

static void ls2d_position_component_init(Ls2DPositionComponent *self)
{
        self->parent.comp_id = LS2D_COMP_ID_POSITION;
}

Ls2DPositionComponent *ls2d_position_component_unref(Ls2DPositionComponent *self)
{
        return ls2d_object_unref(self);
}

void ls2d_position_component_set_xy(Ls2DPositionComponent *self, SDL_Point pos)
{
        if (ls_unlikely(!self)) {
                return;
        }
        self->pos = pos;
}

void ls2d_position_component_set_z(Ls2DPositionComponent *self, int z)
{
        if (ls_unlikely(!self)) {
                return;
        }
        self->pos_z = z;
}

bool ls2d_position_component_get_z(Ls2DPositionComponent *self, int *z)
{
        if (ls_unlikely(!self) || ls_unlikely(!z)) {
                return false;
        }
        *z = self->pos_z;
        return true;
}

bool ls2d_position_component_get_xy(Ls2DPositionComponent *self, SDL_Point *pos)
{
        if (ls_unlikely(!self) || ls_unlikely(!pos)) {
                return false;
        }
        *pos = self->pos;
        return true;
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
