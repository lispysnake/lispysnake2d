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

#define LS2D_TILE_FLIPPED_HORIZONTALLY 0x80000000
#define LS2D_TILE_FLIPPED_VERTICALLY 0x40000000
#define LS2D_TILE_FLIPPED_DIAGONALLY 0x20000000
#define LS2D_TILE_MASK                                                                             \
        ~(LS2D_TILE_FLIPPED_HORIZONTALLY | LS2D_TILE_FLIPPED_VERTICALLY |                          \
          LS2D_TILE_FLIPPED_DIAGONALLY)

struct Ls2DTileMap {
        Ls2DEntity parent;
        Ls2DTileSheet *sheet;
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
        if (ls_likely(self->sheet) != NULL) {
                ls2d_tile_sheet_unref(self->sheet);
        }

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
                return NULL;
        }

        const int index = x + self->width * y;
        if (index > self->size) {
                return NULL;
        }
        return &layer->tiles[index];
}

static bool ls2d_tilemap_get_tile(Ls2DTileMap *self, int layer_index, int x, int y, Ls2DTile *tile)
{
        uint32_t *gid = ls2d_tilemap_get(self, layer_index, x, y);
        if (ls_unlikely(!gid)) {
                return false;
        }
        const uint32_t r_gid = *gid;
        tile->flipped_horizontal = r_gid & LS2D_TILE_FLIPPED_HORIZONTALLY ? true : false;
        tile->flipped_vertical = r_gid & LS2D_TILE_FLIPPED_VERTICALLY ? true : false;
        tile->flipped_diagonal = r_gid & LS2D_TILE_FLIPPED_DIAGONALLY ? true : false;
        tile->gid = r_gid & LS2D_TILE_MASK;

        return true;
}

bool ls2d_tilemap_set_tile(Ls2DTileMap *self, int layer_index, int x, int y, Ls2DTile tile)
{
        uint32_t *t = ls2d_tilemap_get(self, layer_index, x, y);
        if (ls_unlikely(!t)) {
                return false;
        }
        uint32_t new_gid = tile.gid;
        if (tile.flipped_horizontal) {
                new_gid |= LS2D_TILE_FLIPPED_HORIZONTALLY;
        }
        if (tile.flipped_vertical) {
                new_gid |= LS2D_TILE_FLIPPED_VERTICALLY;
        }
        if (tile.flipped_diagonal) {
                new_gid |= LS2D_TILE_FLIPPED_DIAGONALLY;
        }
        *t = new_gid;
        return true;
}

static void ls2d_tilemap_draw(Ls2DEntity *entity, Ls2DTextureCache *cache, Ls2DFrameInfo *frame)
{
        Ls2DTileMap *self = (Ls2DTileMap *)entity;
        Ls2DTile tile = { 0 };
        SDL_Rect draw_area = { 0 };

        int x_start = 0;
        int y_start = 0;
        int x_draw = x_start;
        int y_draw = y_start;

        int current_row = 0;
        int current_col = 0;
        int max_row = self->height;
        int max_col = self->width;

        /* Grab the draw area (renderable section of camera) */
        if (ls2d_camera_get_view(frame->camera, &draw_area)) {
                x_start = 0 - draw_area.x;
                x_draw = x_start;
                y_start = 0 - draw_area.y;
                y_draw = y_start;
        }

        for (uint16_t i = 0; i < self->layers->len; i++) {
                Ls2DTileMapLayer *layer = lookup_layer(self->layers->data, i);

                for (uint16_t y = current_row; y < max_row; y++) {
                        for (uint16_t x = current_col; x < max_col; x++) {
                                Ls2DTextureNode *node = NULL;
                                Ls2DTextureHandle handle;
                                SDL_Rect area = { .w = self->tile_size,
                                                  .h = self->tile_size,
                                                  .x = x_draw,
                                                  .y = y_draw };
                                if (!ls2d_tilemap_get_tile(self, i, x, y, &tile)) {
                                        fprintf(stderr, "Missing tile??!\n");
                                        abort();
                                }
                                handle =
                                    ls2d_tile_sheet_lookup(self->sheet, LS_INT_TO_PTR(tile.gid));
                                node = ls2d_texture_cache_lookup(cache, frame, handle);
                                if (tile.gid == 0 || !node) {
                                        SDL_SetRenderDrawColor(frame->renderer, 255, 255, 255, 255);
                                        SDL_RenderDrawRect(frame->renderer, &area);
                                        goto draw_next;
                                }

                                SDL_RenderCopyEx(frame->renderer,
                                                 node->texture,
                                                 node->subregion ? &node->area : NULL,
                                                 &area,
                                                 0,
                                                 NULL,
                                                 0);
                        draw_next:
                                x_draw += self->tile_size;
                        }
                        /* Increment to next row */
                        y_draw += self->tile_size;
                        x_draw = x_start;
                }
        }
}

void ls2d_tilemap_set_tilesheet(Ls2DTileMap *self, Ls2DTileSheet *sheet)
{
        if (ls_unlikely(!self)) {
                return;
        }
        self->sheet = ls2d_object_ref(sheet);
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
