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
#include "tilemap-private.h"

static void ls2d_tilemap_destroy(Ls2DTileMap *self);
static void ls2d_tilemap_init(Ls2DTileMap *self);
static void ls2d_tilemap_draw(Ls2DEntity *entity, Ls2DTextureCache *cache, Ls2DFrameInfo *frame);
static void ls2d_tilemap_update(Ls2DEntity *entity, Ls2DTextureCache *cache, Ls2DFrameInfo *frame);

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable tilemap_vtable = {
        .obj_name = "Ls2DTileMap",
        .init = (ls2d_object_vfunc_init)ls2d_tilemap_init,
        .destroy = (ls2d_object_vfunc_destroy)ls2d_tilemap_destroy,
};

static Ls2DTileMap *ls2d_tilemap_new_internal(void)
{
        return LS2D_NEW(Ls2DTileMap, tilemap_vtable);
}

static void ls2d_tilemap_init(Ls2DTileMap *self)
{
        self->layers = ls_array_new_size(sizeof(struct Ls2DTileMapLayer), 5);
        self->tilesheets = ls_ptr_array_new_size(1);
        self->parent.draw = ls2d_tilemap_draw;
        self->parent.update = ls2d_tilemap_update;
}

Ls2DEntity *ls2d_tilemap_new(int tile_size, uint16_t width, uint16_t height)
{
        Ls2DTileMap *self = NULL;

        self = ls2d_tilemap_new_internal();
        if (ls_unlikely(!self)) {
                return NULL;
        }

        self->width = width;
        self->height = height;
        self->tile_size = tile_size;
        self->size = self->width * self->height;

        return (Ls2DEntity *)self;
}

Ls2DEntity *ls2d_tilemap_new_from_tmx(Ls2DTextureCache *cache, const char *filename)
{
        Ls2DTileMap *self = NULL;

        self = ls2d_tilemap_new_internal();
        if (ls_unlikely(!self)) {
                return NULL;
        }

        if (!ls2d_tilemap_load_tmx(self, cache, filename)) {
                ls2d_tilemap_destroy(self);
                free(self);
                return NULL;
        }

        return (Ls2DEntity *)self;
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
        if (ls_likely(self->layers != NULL)) {
                for (uint16_t i = 0; i < self->layers->len; i++) {
                        Ls2DTileMapLayer *layer = lookup_layer(self->layers->data, i);
                        ls2d_tilemap_free_layer(layer);
                }
                ls_array_free(self->layers, NULL);
        }

        if (ls_likely(self->tilesheets != NULL)) {
                ls_array_free(self->tilesheets, ls2d_tile_sheet_unref);
        }
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

bool ls2d_tilemap_set_internal(Ls2DTileMap *self, int layer_index, int x, int y, uint32_t gid)
{
        uint32_t *t = ls2d_tilemap_get(self, layer_index, x, y);
        if (ls_unlikely(!t)) {
                return false;
        }
        *t = gid;
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

static Ls2DTextureNode *ls2d_tilemap_find_texture_node(Ls2DTileMap *self, Ls2DTextureCache *cache,
                                                       Ls2DFrameInfo *frame, uint32_t gid)
{
        for (uint16_t i = 0; i < self->tilesheets->len; i++) {
                Ls2DTextureHandle handle = 0;
                Ls2DTileSheet *sheet = (Ls2DTileSheet *)self->tilesheets->data[i];

                handle = ls2d_tile_sheet_lookup(sheet, gid);
                if (handle < 1) {
                        continue;
                }
                return ls2d_texture_cache_lookup(cache, frame, handle);
        }
        return NULL;
}

static void ls2d_tilemap_draw(Ls2DEntity *entity, Ls2DTextureCache *cache, Ls2DFrameInfo *frame)
{
        Ls2DTileMap *self = (Ls2DTileMap *)entity;
        SDL_Rect draw_area = { 0 };
        int x_draw, y_draw = 0;

        for (uint16_t i = 0; i < self->layers->len; i++) {
                Ls2DTileMapLayer *layer = lookup_layer(self->layers->data, i);
                x_draw = self->render.x_start;
                y_draw = self->render.y_start;

                for (uint16_t y = self->render.first_row; y < self->render.max_row; y++) {
                        for (uint16_t x = self->render.first_column; x < self->render.max_column;
                             x++) {
                                Ls2DTextureNode *node = NULL;
                                Ls2DTextureHandle handle;
                                Ls2DTile tile = { 0 };

                                SDL_Rect area = { .w = self->tile_size,
                                                  .h = self->tile_size,
                                                  .x = x_draw,
                                                  .y = y_draw };
                                if (!ls2d_tilemap_get_tile(self, i, x, y, &tile)) {
                                        fprintf(stderr, "Missing tile??!\n");
                                        abort();
                                }

                                /* Draw outline texture for layer 0 */
                                if (tile.gid == 0) {
                                        goto draw_next;
                                }

                                node = ls2d_tilemap_find_texture_node(self, cache, frame, tile.gid);
                                if (ls_unlikely(node == NULL)) {
                                        goto draw_next;
                                }

                                /* Non-regular tiles, anchor to bottom of cell */
                                if (!node->subregion) {
                                        area.w = node->area.w;
                                        area.h = node->area.h;
                                        if (node->area.w != self->tile_size ||
                                            node->area.h != self->tile_size) {
                                                area.y += self->tile_size;
                                                area.y -= node->area.h;
                                        }
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
                        x_draw = self->render.x_start;
                }
        }
}

void ls2d_tilemap_add_tilesheet(Ls2DTileMap *self, Ls2DTileSheet *sheet)
{
        if (ls_unlikely(!self)) {
                return;
        }
        ls_array_add(self->tilesheets, ls2d_object_ref(sheet));
}

static void ls2d_tilemap_update(Ls2DEntity *entity, Ls2DTextureCache *cache, Ls2DFrameInfo *frame)
{
        Ls2DTileMap *self = (Ls2DTileMap *)entity;
        SDL_Rect draw_area = { 0 };

        int visible_columns = 0;
        int visible_rows = 0;

        /* Basic Premise:
         * Work out the visible rows and columns for the draw area
         * Only draw those columns
         * Draw at offset to camera and missing columns
         * TODO: Optimise by only changing the camera when it needs changing.
         */

        for (uint16_t i = 0; i < self->tilesheets->len; i++) {
                Ls2DTileSheet *sheet = (Ls2DTileSheet *)self->tilesheets->data[i];
                ls2d_tile_sheet_update(sheet, frame);
        }

        if (!ls2d_camera_get_view(frame->camera, &draw_area)) {
                self->render.first_column = 0;
                self->render.first_row = 0;
                self->render.max_row = self->height;
                self->render.max_column = self->width;
                self->render.x_start = 0;
                self->render.y_start = 0;
                return;
        }

        visible_columns = ceil((float)draw_area.w / self->tile_size);
        visible_rows = ceil((float)draw_area.h / self->tile_size);

        self->render.first_row = floor((float)draw_area.y / self->tile_size);
        self->render.first_column = floor((float)draw_area.x / self->tile_size);

        /* TODO: Introduce MAX/MIN macros */
        self->render.max_column = self->render.first_column + visible_columns + 1;
        self->render.max_row = self->render.first_row + visible_rows + 1;
        if (self->render.max_row > self->height) {
                self->render.max_row = self->height;
        }
        if (self->render.max_column > self->width) {
                self->render.max_column = self->width;
        }

        self->render.x_start = 0 - draw_area.x;
        self->render.x_start += self->render.first_column * self->tile_size;

        self->render.y_start = 0 - draw_area.y;
        self->render.y_start += self->render.first_row * self->tile_size;
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
