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

static void ls2d_entity_destroy(Ls2DEntity *self);

struct Ls2DEntity {
        Ls2DObject parent;

        const char *name;
        bool had_init;

        /* components storage */
        LsPtrArray *components;
};

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable entity_vtable = {
        .destroy = (ls2d_object_vfunc_destroy)ls2d_entity_destroy,
        .obj_name = "Ls2DEntity",
};

Ls2DEntity *ls2d_entity_new(const char *name)
{
        Ls2DEntity *self = NULL;

        self = calloc(1, sizeof(struct Ls2DEntity));
        if (ls_unlikely(!self)) {
                return NULL;
        }
        self->components = ls_ptr_array_new();
        if (ls_unlikely(!self->components)) {
                free(self);
                return NULL;
        }

        /* Scene name for traversal */
        self->name = name;

        return ls2d_object_init((Ls2DObject *)self, &entity_vtable);
}

Ls2DEntity *ls2d_entity_unref(Ls2DEntity *self)
{
        return ls2d_object_unref(self);
}

static inline void free_component(void *v)
{
        (void)ls2d_component_unref(v);
}

static void ls2d_entity_destroy(Ls2DEntity *self)
{
        ls_array_free(self->components, free_component);
}

void ls2d_entity_add_component(Ls2DEntity *self, Ls2DComponent *component)
{
        if (ls_unlikely(!self) || ls_unlikely(!component)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ls2DEntity not correctly initialised");
                return;
        }
        ls_array_add(self->components, ls2d_object_ref(component));
}

/**
 * Inform the entity that all components need to draw now
 */
void ls2d_entity_draw(Ls2DEntity *self, Ls2DTextureCache *cache, Ls2DFrameInfo *frame)
{
        for (uint16_t i = 0; i < self->components->len; i++) {
                Ls2DComponent *comp = self->components->data[i];
                ls2d_component_draw(comp, cache, frame);
        }
}

/**
 * Inform the entity that all components need to update now.
 */
void ls2d_entity_update(Ls2DEntity *self, Ls2DTextureCache *cache, Ls2DFrameInfo *frame)
{
        bool had_init = self->had_init;
        for (uint16_t i = 0; i < self->components->len; i++) {
                Ls2DComponent *comp = self->components->data[i];
                if (!had_init) {
                        ls2d_component_init(comp, cache, frame);
                }
                ls2d_component_update(comp, cache, frame);
        }
        if (!had_init) {
                self->had_init = true;
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
