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

#include <libxml/xmlreader.h>

#include "ls2d.h"

/**
 * Opaque Ls2DTileSheet implementation
 */
struct Ls2DTileSheet {
        Ls2DObject object; /*< Parent */

        Ls2DTextureCache *cache;
        LsPtrArray *animations;
        LsArray *texture_objs;
};

/**
 * Each cell in the table has a handle and a potential animation.
 */
typedef struct Ls2DTileSheetCell {
        Ls2DTextureHandle handle;
        Ls2DAnimation *animation;
} Ls2DTileSheetCell;

/**
 * Private API headers for the Ls2DTileSheet implementation
 */

/**
 * TSX instance parser
 */
typedef struct Ls2DTileSheetTSX {
        bool in_tileset;
        bool in_image;
        bool in_grid;
        bool in_tile;
        bool in_animation;
        bool in_frame;
        bool sheet; /**<Whether this is a simple tilesheet */
        Ls2DAnimation *animation;
        struct {
                int width;   /**< Tile height */
                int height;  /**< Tile width */
                int count;   /**< How many tiles? */
                int columns; /**<How many across is it? */
                int spacing; /**<Spacing between tiles */
                int margin;  /**<How much margin is present in the main image */
        } tileset;
        struct {
                int id;
        } tile;
        struct {
                int orientation;
                int width;
                int height;
        } grid;
        struct {
                int width;
                int height;
                char *source;
        } image;
} Ls2DTileSheetTSX;

__attribute__((always_inline)) static inline Ls2DTileSheetCell *ls2d_tile_sheet_get_cell(
    void *cache, uint32_t index)
{
        Ls2DTileSheetCell *root = cache;
        return &(root[index]);
}

/**
 * Attempt to load XML file.
 */
bool ls2d_tile_sheet_parse_tsx(Ls2DTileSheet *self, const char *filename);

DEF_AUTOFREE(xmlTextReader, xmlFreeTextReader)
DEF_AUTOFREE(xmlChar, xmlFree)

static inline void ls2d_tile_sheet_get_int_attr(xmlTextReader *reader, int *storage, const char *id)
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
