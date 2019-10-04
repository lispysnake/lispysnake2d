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

#include <SDL.h>

#include "ls2d.h"

/**
 * Helper type to help set and retrieve tile cell attributes
 */
struct Ls2DTile {
        uint32_t gid;
        bool flipped_horizontal;
        bool flipped_diagonal;
        bool flipped_vertical;
};

/**
 * Construct a new Ls2DTileMap
 */
Ls2DEntity *ls2d_tilemap_new(int tilesize, uint16_t map_width, uint16_t map_height);

/**
 * Construct a new Ls2DTileMap from a TMX file
 */
Ls2DEntity *ls2d_tilemap_new_from_tmx(const char *filename);

/**
 * Attempt to insert a new layer in the map
 */
bool ls2d_tilemap_add_layer(Ls2DTileMap *map, int render_index);

/**
 * Set the gid in the given layer, at X by Y
 */
bool ls2d_tilemap_set_tile(Ls2DTileMap *map, int layer_index, int x, int y, Ls2DTile tile);

void ls2d_tilemap_set_tilesheet(Ls2DTileMap *map, Ls2DTileSheet *tilesheet);

/**
 * Unref a previously allocated tileMap
 */
Ls2DTileMap *ls2d_tilemap_unref(Ls2DTileMap *self);

DEF_AUTOFREE(Ls2DTileMap, ls2d_tilemap_unref)

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
