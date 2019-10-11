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

#include "tilesheet-private.h"

static void ls2d_tile_sheet_destroy(Ls2DTileSheet *self);
static void ls2d_tile_sheet_destroy_cell(Ls2DTileSheetCell *cell);

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable tile_sheet_vtable = {
        .destroy = (ls2d_object_vfunc_destroy)ls2d_tile_sheet_destroy,
        .obj_name = "Ls2DTileSheet",
};

static Ls2DTileSheet *ls2d_tile_sheet_new_internal(Ls2DTextureCache *cache, bool free_keys)
{
        Ls2DTileSheet *self = NULL;

        self = calloc(1, sizeof(struct Ls2DTileSheet));
        if (ls_unlikely(!self)) {
                return NULL;
        }

        if (free_keys) {
                self->textures = ls_hashmap_new_full(ls_hashmap_string_hash,
                                                     ls_hashmap_string_equal,
                                                     free,
                                                     ls2d_tile_sheet_destroy_cell);
        } else {
                self->textures = ls_hashmap_new_full(ls_hashmap_simple_hash,
                                                     ls_hashmap_simple_equal,
                                                     NULL,
                                                     ls2d_tile_sheet_destroy_cell);
        }

        if (ls_unlikely(!self->textures)) {
                free(self);
                return NULL;
        }
        self->animations = ls_ptr_array_new();
        if (ls_unlikely(!self->animations)) {
                free(self);
                return NULL;
        }
        self->cache = cache;

        return ls2d_object_init((Ls2DObject *)self, &tile_sheet_vtable);
}

Ls2DTileSheet *ls2d_tile_sheet_new_from_xml(Ls2DTextureCache *cache, const char *file_path)
{
        Ls2DTileSheet *self = NULL;

        self = ls2d_tile_sheet_new_internal(cache, true);
        if (!self) {
                return NULL;
        }

        if (!ls2d_tile_sheet_parse_xml(self, file_path)) {
                ls2d_tile_sheet_unref(self);
                return NULL;
        }

        return self;
}

Ls2DTileSheet *ls2d_tile_sheet_new_from_tsx(Ls2DTextureCache *cache, const char *file_path)
{
        Ls2DTileSheet *self = NULL;

        self = ls2d_tile_sheet_new_internal(cache, false);
        if (!self) {
                return NULL;
        }

        if (!ls2d_tile_sheet_parse_tsx(self, file_path)) {
                ls2d_tile_sheet_unref(self);
                return NULL;
        }

        return self;
}

Ls2DTileSheet *ls2d_tile_sheet_unref(Ls2DTileSheet *self)
{
        return ls2d_object_unref(self);
}

static void ls2d_tile_sheet_destroy(Ls2DTileSheet *self)
{
        ls_hashmap_free(self->textures);
        ls_array_free(self->animations, NULL);
}

static void ls2d_tile_sheet_destroy_cell(Ls2DTileSheetCell *cell)
{
        if (!cell) {
                return;
        }
        if (ls_unlikely(cell->animation != NULL)) {
                ls2d_animation_unref(cell->animation);
        }
        free(cell);
}

bool ls2d_tile_sheet_put_handle(Ls2DTileSheet *self, void *key, Ls2DTextureHandle handle)
{
        Ls2DTileSheetCell *cell = NULL;

        cell = calloc(1, sizeof(struct Ls2DTileSheetCell));
        if (ls_unlikely(!cell)) {
                return false;
        }
        cell->handle = handle;
        fprintf(stderr, "ID: %d Put handle %d\n", LS_PTR_TO_INT(key), handle);
        return ls_hashmap_put(self->textures, key, cell);
}

bool ls2d_tile_sheet_put_animation(Ls2DTileSheet *self, void *key, Ls2DAnimation *animation)
{
        Ls2DTileSheetCell *cell = NULL;
        Ls2DTextureHandle cur_handle = 0;

        cur_handle = ls2d_animation_get_texture(animation);

        cell = ls_hashmap_get(self->textures, key);
        if (!cell && !ls2d_tile_sheet_put_handle(self, key, cur_handle)) {
                return false;
        }
        cell->handle = cur_handle;
        cell->animation = animation;
        return true;
}

bool ls2d_tile_sheet_lookup(Ls2DTileSheet *self, void *key, Ls2DTextureHandle *handle)
{
        Ls2DTileSheetCell *cell = NULL;

        if (ls_unlikely(!self)) {
                return 0;
        }

        cell = ls_hashmap_get(self->textures, key);
        if (ls_unlikely(!cell)) {
                *handle = 0;
                return false;
        }
        if (cell->animation) {
                *handle = ls2d_animation_get_texture(cell->animation);
        } else {
                *handle = cell->handle;
        }
        return true;
}

Ls2DTextureHandle ls2d_tile_sheet_get(Ls2DTileSheet *self, void *key)
{
        Ls2DTextureHandle ret = 0;
        if (!ls2d_tile_sheet_lookup(self, key, &ret)) {
                return 0;
        }
        return ret;
}

void ls2d_tile_sheet_update(Ls2DTileSheet *self, Ls2DFrameInfo *frame)
{
        if (ls_unlikely(!self)) {
                return;
        }
        for (uint16_t i = 0; i < self->animations->len; i++) {
                Ls2DAnimation *animation = (Ls2DAnimation *)self->animations->data[i];
                ls2d_animation_update(animation, frame);
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
