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

#pragma once

#include <fcntl.h>
#include <libxml/xmlreader.h>
#include <unistd.h>

#include "ls2d.h"

#define LS2D_TILE_FLIPPED_HORIZONTALLY 0x80000000
#define LS2D_TILE_FLIPPED_VERTICALLY 0x40000000
#define LS2D_TILE_FLIPPED_DIAGONALLY 0x20000000
#define LS2D_TILE_MASK                                                                             \
        ~(LS2D_TILE_FLIPPED_HORIZONTALLY | LS2D_TILE_FLIPPED_VERTICALLY |                          \
          LS2D_TILE_FLIPPED_DIAGONALLY)

struct Ls2DTileMap {
        Ls2DEntity parent;
        int tile_size;
        uint16_t width;
        uint16_t height;
        LsArray *layers;     /**<An array of Ls2DTileMapLayer */
        LsArray *tilesheets; /**<An array of tilesheets */
        int size;

        struct {
                int first_column;
                int first_row;
                int max_row;
                int max_column;

                int x_start;
                int y_start;
        } render;
};

typedef struct Ls2DTileMapLayer {
        int render_index; /**<TODO: Shorten to uint8_t */
        uint32_t *tiles;
} Ls2DTileMapLayer;

/**
 * XML instance parser
 */
typedef struct Ls2DTileMapTMX {
        bool in_map;
        bool in_layer;
        bool in_data;

        struct {
                int orientation;
                int width;
                int height;
                int tile_width;
                int tile_height;
                /* TODO: Support infinite.. ?*/
        } map;

        struct {
                int first_gid;
                char *source;
        } tileset;

        struct {
                int id;
                char *name;
                int width;
                int height;
        } layer;

        Ls2DTextureCache *cache;
} Ls2DTileMapTMX;

bool ls2d_tilemap_load_tsx(Ls2DTileMap *self, Ls2DTextureCache *cache, const char *filename);
bool ls2d_tilemap_set_internal(Ls2DTileMap *self, int layer_index, int x, int y, uint32_t gid);

DEF_AUTOFREE(xmlTextReader, xmlFreeTextReader)
DEF_AUTOFREE(xmlChar, xmlFree)
DEF_AUTOFREE(char, free)

static inline void ls2d_tilemap_get_int_attr(xmlTextReader *reader, int *storage, const char *id)
{
        autofree(xmlChar) *attr = NULL;

        /* TODO: Proper error checking of atoi, etc. */
        attr = xmlTextReaderGetAttribute(reader, BAD_CAST id);
        if (!attr) {
                *storage = 0;
                return;
        }
        *storage = atoi((const char *)attr);
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
