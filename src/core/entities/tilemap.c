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

struct Ls2DTileMap {
        Ls2DEntity parent;
        int tile_size;
        uint16_t width;
        uint16_t height;
        LsArray *layers; /**<An array of Ls2DTileMapLayer */
        int size;
};

typedef struct Ls2DTileMapLayer {
        int render_index; /**<TODO: Shorten to uint8_t */
        uint32_t *tiles;
} Ls2DTileMapLayer;

static void ls2d_tilemap_destroy(Ls2DTileMap *self);
static void ls2d_tilemap_draw(Ls2DEntity *entity, Ls2DTextureCache *cache, Ls2DFrameInfo *frame);
static void ls2d_tilemap_update(Ls2DEntity *entity, Ls2DTextureCache *cache, Ls2DFrameInfo *frame);

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable tilemap_vtable = {
        .obj_name = "Ls2DTileMap",
        .destroy = (ls2d_object_vfunc_destroy)ls2d_tilemap_destroy,
};

Ls2DEntity *ls2d_tilemap_new(int tile_size, uint16_t width, uint16_t height)
{
        Ls2DTileMap *self = NULL;

        self = calloc(1, sizeof(struct Ls2DTileMap));
        if (ls_unlikely(!self)) {
                return NULL;
        }
        self->layers = ls_array_new_size(sizeof(struct Ls2DTileMapLayer), 5);
        if (ls_unlikely(!self->layers)) {
                goto bail;
        }

        self->parent.draw = ls2d_tilemap_draw;
        self->parent.update = ls2d_tilemap_update;
        self->width = width;
        self->height = height;
        self->tile_size = tile_size;
        self->size = self->width * self->height;

        if (ls_unlikely(!ls2d_tilemap_add_layer(self, 0))) {
                goto bail;
        }

        return (Ls2DEntity *)ls2d_object_init((Ls2DObject *)self, &tilemap_vtable);

bail:
        ls2d_tilemap_destroy(self->layers);
        free(self);
        return NULL;
}

Ls2DTileMap *ls2d_tilemap_unref(Ls2DTileMap *self)
{
        return ls2d_object_unref(self);
}

__attribute__((always_inline)) static inline Ls2DTileMapLayer *lookup_layer(void *cache, int index)
{
        Ls2DTileMapLayer *root = cache;
        return &(root[index]);
}

static void ls2d_tilemap_free_layer(void *v)
{
        Ls2DTileMapLayer *layer = v;
        if (ls_likely(layer->tiles != NULL)) {
                free(layer->tiles);
        }
}

static void ls2d_tilemap_destroy(Ls2DTileMap *self)
{
        if (ls_unlikely(!self->layers)) {
                return;
        }

        for (uint16_t i = 0; i < self->layers->len; i++) {
                Ls2DTileMapLayer *layer = lookup_layer(self->layers->data, i);
                ls2d_tilemap_free_layer(layer);
        }
        ls_array_free(self->layers, NULL);
}

bool ls2d_tilemap_add_layer(Ls2DTileMap *self, int render_index)
{
        Ls2DTileMapLayer *layer = NULL;

        if (ls_unlikely(!self)) {
                return false;
        }
        if (!ls_array_add(self->layers, NULL)) {
                return false;
        }
        uint16_t index = (uint32_t)(self->layers->len - 1);
        layer = lookup_layer(self->layers->data, index);
        layer->render_index = render_index;
        layer->tiles = calloc(sizeof(uint32_t), self->width * self->height);
        if (ls_unlikely(!layer->tiles)) {
                return false;
        }
        return true;
}

static inline uint32_t *ls2d_tilemap_get(Ls2DTileMap *self, int layer_index, int x, int y)
{
        Ls2DTileMapLayer *layer = NULL;
        if (ls_unlikely(!self)) {
                return NULL;
        }

        layer = lookup_layer(self->layers->data, layer_index);
        if (ls_unlikely(!layer)) {
                return false;
        }

        const int index = x + self->width * y;
        if (index > self->size) {
                return NULL;
        }
        return &layer->tiles[index];
}

bool ls2d_tilemap_set(Ls2DTileMap *self, int layer_index, int x, int y, uint32_t gid)
{
        uint32_t *tile = ls2d_tilemap_get(self, layer_index, x, y);
        if (ls_unlikely(!tile)) {
                return false;
        }
        *tile = gid;
        return true;
}

static void ls2d_tilemap_draw(Ls2DEntity *entity, Ls2DTextureCache *cache, Ls2DFrameInfo *frame)
{
        Ls2DTileMap *self = (Ls2DTileMap *)entity;

        for (uint16_t i = 0; i < self->layers->len; i++) {
                Ls2DTileMapLayer *layer = lookup_layer(self->layers->data, i);

                for (uint16_t x = 0; x < self->width; x++) {
                        for (uint16_t y = 0; y < self->height; y++) {
                                uint32_t tile = layer->tiles[x + self->width * y];
                                fprintf(stderr, "%d ", tile);
                        }
                        fprintf(stderr, "\n");
                }
        }
}

static void ls2d_tilemap_update(Ls2DEntity *entity, Ls2DTextureCache *cache, Ls2DFrameInfo *frame)
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
