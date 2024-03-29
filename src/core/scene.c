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

#include <SDL.h>
#include <stdlib.h>
#include <string.h>

#include "ls2d.h"

static void ls2d_scene_init(Ls2DScene *self);
static void ls2d_scene_destroy(Ls2DScene *self);

/**
 * Opaque Ls2DScene implementation
 */
struct Ls2DScene {
        Ls2DObject object; /*< Parent */
        const char *name;

        LsPtrArray *entities;        /**<Our list of entities to render. */
        LsHashmap *cameras;          /**<Our set of cameras */
        Ls2DTextureCache *tex_cache; /**< Our private texture cache. */
        Ls2DCamera *active_camera;
};

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable scene_vtable = {
        .init = (ls2d_object_vfunc_init)ls2d_scene_init,
        .destroy = (ls2d_object_vfunc_destroy)ls2d_scene_destroy,
        .obj_name = "Ls2DScene",
};

Ls2DScene *ls2d_scene_new(void)
{
        return LS2D_NEW(Ls2DScene, scene_vtable);
}

static void ls2d_scene_init(Ls2DScene *self)
{
        self->entities = ls_ptr_array_new();
        self->tex_cache = ls2d_texture_cache_new();
        self->cameras = ls_hashmap_new_full(ls_hashmap_string_hash,
                                            ls_hashmap_string_equal,
                                            free,
                                            (ls_hashmap_free_func)ls2d_camera_unref);
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
        if (ls_likely(self->entities != NULL)) {
                ls_array_free(self->entities, free_entity);
        }
        if (ls_likely(self->tex_cache != NULL)) {
                ls2d_texture_cache_unref(self->tex_cache);
        }
        if (ls_likely(self->cameras != NULL)) {
                ls_hashmap_free(self->cameras);
        }
}

const char *ls2d_scene_get_name(Ls2DScene *self)
{
        if (ls_unlikely(!self)) {
                return NULL;
        }
        return self->name;
}

Ls2DTextureCache *ls2d_scene_get_texture_cache(Ls2DScene *self)
{
        if (ls_unlikely(!self)) {
                return NULL;
        }
        return self->tex_cache;
}

static bool ls2d_scene_should_render(Ls2DScene *self, Ls2DEntity *entity)
{
        if (ls_unlikely(!entity)) {
                return false;
        }
        if (ls_unlikely(!self->active_camera)) {
                return true;
        }
        if (!ls2d_camera_entity_in_bounds(self->active_camera, entity)) {
                return false;
        }
        return true;
}

void ls2d_scene_add_entity(Ls2DScene *self, Ls2DEntity *entity)
{
        if (ls_unlikely(!self) || ls_unlikely(!entity)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ls2DScene not yet initialised");
                return;
        }

        /* Insert entity into our list */
        ls_array_add(self->entities, ls2d_object_ref(entity));
}

void ls2d_scene_draw(Ls2DScene *self, Ls2DFrameInfo *frame)
{
        for (uint16_t i = 0; i < self->entities->len; i++) {
                Ls2DEntity *entity = self->entities->data[i];
                if (!ls2d_scene_should_render(self, entity)) {
                        continue;
                }
                ls2d_entity_draw(entity, self->tex_cache, frame);
        }
}

void ls2d_scene_update(Ls2DScene *self, Ls2DFrameInfo *frame)
{
        if (ls_likely(self->active_camera != NULL)) {
                ls2d_camera_update(self->active_camera, frame);
                frame->camera = self->active_camera;
        } else {
                frame->camera = NULL;
        }

        for (uint16_t i = 0; i < self->entities->len; i++) {
                Ls2DEntity *entity = self->entities->data[i];
                ls2d_entity_update(entity, self->tex_cache, frame);
        }
}

bool ls2d_scene_add_camera(Ls2DScene *self, const char *id, Ls2DCamera *camera)
{
        if (ls_unlikely(!self) || ls_unlikely(!camera)) {
                return false;
        }

        char *id_c = strdup(id);
        if (ls_unlikely(!id_c)) {
                return false;
        }

        if (ls_unlikely(!ls_hashmap_put(self->cameras, id_c, ls2d_object_ref(camera)))) {
                ls2d_camera_unref(camera);
                free(id_c);
                return false;
        }

        if (ls_unlikely(!self->active_camera)) {
                self->active_camera = camera;
        }

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
