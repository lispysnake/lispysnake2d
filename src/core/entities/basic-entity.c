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

static void ls2d_basic_entity_init(Ls2DBasicEntity *self);
static void ls2d_basic_entity_destroy(Ls2DBasicEntity *self);
static void ls2d_basic_entity_draw(Ls2DEntity *entity, Ls2DTextureCache *cache,
                                   Ls2DFrameInfo *frame);
static void ls2d_basic_entity_update(Ls2DEntity *entity, Ls2DTextureCache *cache,
                                     Ls2DFrameInfo *frame);
static void ls2d_basic_entity_add_component(Ls2DEntity *entity, Ls2DComponent *component);
static Ls2DComponent *ls2d_basic_entity_get_component(Ls2DEntity *entity, int component_id);

struct Ls2DBasicEntity {
        Ls2DEntity parent;

        const char *name;
        bool had_init;

        /* components storage */
        LsPtrArray *components;
};

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable entity_vtable = {
        .init = (ls2d_object_vfunc_init)ls2d_basic_entity_init,
        .destroy = (ls2d_object_vfunc_destroy)ls2d_basic_entity_destroy,
        .obj_name = "Ls2DBasicEntity",
};

Ls2DEntity *ls2d_basic_entity_new(const char *name)
{
        Ls2DBasicEntity *self = LS2D_NEW(Ls2DBasicEntity, entity_vtable);
        if (ls_unlikely(!self)) {
                return NULL;
        }
        self->name = name;
        return (Ls2DEntity *)self;
}

static void ls2d_basic_entity_init(Ls2DBasicEntity *self)
{
        self->components = ls_ptr_array_new();
        self->parent.draw = ls2d_basic_entity_draw;
        self->parent.update = ls2d_basic_entity_update;
        self->parent.add_component = ls2d_basic_entity_add_component;
        self->parent.get_component = ls2d_basic_entity_get_component;
}

Ls2DBasicEntity *ls2d_basic_entity_unref(Ls2DBasicEntity *self)
{
        return ls2d_object_unref(self);
}

static inline void free_component(void *v)
{
        (void)ls2d_component_unref(v);
}

static void ls2d_basic_entity_destroy(Ls2DBasicEntity *self)
{
        ls_array_free(self->components, free_component);
}

static void ls2d_basic_entity_add_component(Ls2DEntity *entity, Ls2DComponent *component)
{
        Ls2DBasicEntity *self = (Ls2DBasicEntity *)entity;

        if (ls_unlikely(!self) || ls_unlikely(!component)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ls2DBasicEntity not correctly initialised");
                return;
        }

        ls2d_component_set_parent_entity(component, (Ls2DEntity *)self);
        ls_array_add(self->components, ls2d_object_ref(component));
}

/**
 * Inform the entity that all components need to draw now
 */
static void ls2d_basic_entity_draw(Ls2DEntity *entity, Ls2DTextureCache *cache,
                                   Ls2DFrameInfo *frame)
{
        Ls2DBasicEntity *self = (Ls2DBasicEntity *)entity;

        for (uint16_t i = 0; i < self->components->len; i++) {
                Ls2DComponent *comp = self->components->data[i];
                ls2d_component_draw(comp, cache, frame);
        }
}

/**
 * Inform the entity that all components need to update now.
 */
static void ls2d_basic_entity_update(Ls2DEntity *entity, Ls2DTextureCache *cache,
                                     Ls2DFrameInfo *frame)
{
        Ls2DBasicEntity *self = (Ls2DBasicEntity *)entity;

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

static Ls2DComponent *ls2d_basic_entity_get_component(Ls2DEntity *entity, int component_id)
{
        Ls2DBasicEntity *self = (Ls2DBasicEntity *)entity;

        self = (Ls2DBasicEntity *)entity;
        /* Easier to have a map in future. */
        for (uint16_t i = 0; i < self->components->len; i++) {
                Ls2DComponent *comp = self->components->data[i];
                if (comp->comp_id == component_id) {
                        return comp;
                }
        }
        return NULL;
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
