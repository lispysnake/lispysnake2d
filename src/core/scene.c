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
#include "object.h"
#include "scene.h"

static void ls2d_scene_destroy(Ls2DScene *self);

/**
 * Opaque Ls2DScene implementation
 */
struct Ls2DScene {
        Ls2DObject object; /*< Parent */
        const char *name;

        LsList *entities; /**<Our list of entities to render. */
};

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable scene_vtable = {
        .destroy = (ls2d_object_vfunc_destroy)ls2d_scene_destroy,
        .obj_name = "Ls2DScene",
};

Ls2DScene *ls2d_scene_new(const char *name)
{
        Ls2DScene *self = NULL;

        self = calloc(1, sizeof(struct Ls2DScene));
        if (ls_unlikely(!self)) {
                return NULL;
        }
        /* Scene name for traversal */
        self->name = name;

        return ls2d_object_init((Ls2DObject *)self, &scene_vtable);
}

Ls2DScene *ls2d_scene_unref(Ls2DScene *self)
{
        return ls2d_object_unref(self);
}

static inline void free_entity(void *v)
{
        (void)ls2d_entity_unref(v);
}

static void ls2d_scene_destroy(Ls2DScene *self)
{
        ls_list_free_full(self->entities, free_entity);
}

const char *ls2d_scene_get_name(Ls2DScene *self)
{
        if (ls_unlikely(!self)) {
                return NULL;
        }
        return self->name;
}

void ls2d_scene_add_entity(Ls2DScene *self, Ls2DEntity *entity)
{
        if (ls_unlikely(!self) || ls_unlikely(!entity)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ls2DScene not yet initialised");
                return;
        }

        /* Insert entity into our list */
        self->entities = ls_list_prepend(self->entities, ls2d_object_ref(entity));
}

void ls2d_scene_draw(Ls2DScene *self, Ls2DFrameInfo *frame)
{
        for (LsList *node = self->entities; node != NULL; node = node->next) {
                Ls2DEntity *entity = node->data;
                ls2d_entity_draw(entity, frame);
        }
}

void ls2d_scene_update(Ls2DScene *self, Ls2DFrameInfo *frame)
{
        for (LsList *node = self->entities; node != NULL; node = node->next) {
                Ls2DEntity *entity = node->data;
                ls2d_entity_update(entity, frame);
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
