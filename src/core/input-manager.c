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

static void ls2d_input_manager_destroy(Ls2DInputManager *self);

/**
 * Opaque Ls2DInputManager implementation
 */
struct Ls2DInputManager {
        Ls2DObject object; /*< Parent */
        LsHashmap *key_callbacks;
        LsHashmap *mouse_callbacks;
};

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable input_manager_vtable = {
        .destroy = (ls2d_object_vfunc_destroy)ls2d_input_manager_destroy,
        .obj_name = "Ls2DInputManager",
};

Ls2DInputManager *ls2d_input_manager_new()
{
        Ls2DInputManager *self = NULL;

        self = calloc(1, sizeof(struct Ls2DInputManager));
        if (ls_unlikely(!self)) {
                return NULL;
        }

        self->key_callbacks = ls_hashmap_new(ls_hashmap_simple_hash, ls_hashmap_simple_equal);
        if (ls_unlikely(!self->key_callbacks)) {
                ls2d_input_manager_destroy(self);
                return NULL;
        }

        self->mouse_callbacks = ls_hashmap_new(ls_hashmap_simple_hash, ls_hashmap_simple_equal);
        if (ls_unlikely(!self->mouse_callbacks)) {
                ls2d_input_manager_destroy(self);
                return NULL;
        }

        return ls2d_object_init((Ls2DObject *)self, &input_manager_vtable);
}

Ls2DInputManager *ls2d_input_manager_unref(Ls2DInputManager *self)
{
        return ls2d_object_unref(self);
}

static void ls2d_input_manager_destroy(Ls2DInputManager *self)
{
        if (ls_likely(self->key_callbacks != NULL)) {
                ls_hashmap_free(self->key_callbacks);
        }
        if (ls_likely(self->mouse_callbacks != NULL)) {
                ls_hashmap_free(self->mouse_callbacks);
        }
}

void ls2d_input_manager_process(Ls2DInputManager *self, Ls2DFrameInfo *frame)
{
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
