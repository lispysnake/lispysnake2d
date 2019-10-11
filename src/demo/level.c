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

#include "demo.h"

static const uint32_t gids[] = {
        1178, 1178, 1178, 1178, 1179, 1179, 1219, 1219, 1179, 1179, 1179, 1179, 1179, 1179, 1179,
        1178, 1178, 1178, 1178, 1179, 1218, 1218, 1178, 1179, 1219, 1219, 1178, 1218, 1219, 1219,
        1179, 1179, 1179, 1179, 1179, 1218, 1218, 1178, 1178, 1179, 1178, 1178, 1179, 1219, 1218,
        1218, 1178, 1178, 1178, 1218, 1219, 1219, 1179, 1179, 1178, 1218, 1219, 1218, 1178, 1179,
        1178, 1178, 1178, 1179, 1178, 1178, 1218, 1218, 1218, 1218, 1218, 1218, 1178, 1178, 1218,
        1218, 1219, 1178, 1178, 1179, 1218, 1218, 1218, 1178, 1178, 1178, 1179, 1178, 1178, 1178,
        1178, 1179, 1179, 1218, 1219, 1218, 1218, 1178, 1178, 1179, 1219, 1179, 1179, 1218, 1218,
        1218, 1178, 1179, 1178, 1178, 1178, 1178, 1178, 1218, 1219, 1219, 1218, 1218, 1178, 1179,
        1219, 1219, 1219, 1219, 1219, 1179, 1178, 1178, 1218, 1218, 1218, 1218, 1218, 1219, 1219,
        1219, 1179, 1178, 1179, 1219, 1218, 1178, 1179, 1179, 1179, 1179, 1179, 1218, 1219, 1179,
        1178, 1179, 1218, 1219, 1179, 1218, 1219, 1178, 1179, 1219, 1219, 1178, 1179, 1219, 1219,
        1219, 1178, 1218, 1219, 1179, 1218, 1178, 1179, 1218, 1219, 1179, 1218, 1178, 1179, 1179,
        1178, 1179, 1219, 1219, 1218, 1178, 1218, 1219, 1219, 1219, 1218, 1218, 1178, 1218, 1219,
        1219, 1179, 1178, 1179, 1179, 1178, 1179, 1179, 1178, 1178, 1218, 1219, 1219, 1179, 1179,
        1218, 1219, 1218, 1218, 1219, 1219, 1178, 1178, 1179, 1219, 1218, 1178, 1178, 1218, 1218,
        1218, 1219, 1179, 1219, 1219, 1179, 1218, 1219, 1218, 1219, 1178, 1178, 1178, 1178, 1179,
        1218, 1218, 1218, 1219, 1219, 1219, 1179, 1219, 1179, 1218, 1219, 1178, 1179, 1179, 1219,
        1218, 1178, 1218, 1178, 1179, 1178, 1218, 1218, 1178, 1179, 1179, 1179, 1179, 1179, 1178,
        1179, 1179, 1219, 1219, 1178, 1178, 1218, 1219, 1178, 1179, 1179, 1178, 1178, 1178, 1178,
        1178, 1178, 1178, 1178, 1218, 1219, 1219, 1218, 1219, 1218, 1218, 1219, 1178, 1178, 1179,
        1178, 1179, 1218, 1218, 1218, 1218, 1218, 1218, 1218, 1219, 1218, 1218, 1219, 1218, 1219,
        1218, 1219, 1178, 1178, 1179, 1178, 1179, 1179, 1179, 1179, 1179, 1179, 1179, 1218, 1178,
        1178, 1178, 1178, 1179, 1179, 1179, 1178, 1179, 1178, 1179, 1179, 1179, 1179, 1179, 1179,
        1179, 1179, 1179, 1179, 1179, 1179, 1178, 1179, 1178, 1178, 1179, 1179, 1179, 1178, 1178,
        1178, 1178, 1178, 1178, 1178, 1178, 1178, 1178, 1178, 1178, 1178, 1178, 1178, 1218, 1218,
        1219, 1219, 1179, 1178, 1179, 1218, 1218, 1218, 1218, 1218, 1218, 1218, 1218, 1218, 1218,
        1218, 1218, 1218, 1218, 1219, 1218, 1218, 1219, 1179, 1179
};

static const uint32_t gids_2[] = {
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 7,   8,   9,   10,  11,  0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 47,  48,  49,  50,  51,  0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 87,  88,  89,  90,  91,  0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 127, 128, 129, 130, 131, 0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 167, 168, 169, 170, 171, 0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 12,  13,  14,  15,  16,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 52,  53,  54,  55,  56,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 92,  93,  94,  95,  96,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 132, 133, 134, 135, 136,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 172, 173, 174, 175, 176,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,   0,   0,   0,   0,
        0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,   0,   0,   0,   0
};

bool demo_game_load_tilemap(DemoGame *self)
{
        Ls2DTextureCache *cache = ls2d_scene_get_texture_cache(self->scene);
        Ls2DTileSheet *tsx = NULL;
        // self->tilemap = ls2d_tilemap_new_from_tmx(cache, "data/level1.tmx");
        self->tilemap = ls2d_tilemap_new(16, 20, 20);

        ls2d_tilemap_add_layer((Ls2DTileMap *)self->tilemap, 0);
        ls2d_tilemap_add_layer((Ls2DTileMap *)self->tilemap, 1);

        for (int x = 0; x < 20; x++) {
                for (int y = 0; y < 20; y++) {
                        const int index = x + 20 * y;
                        ls2d_tilemap_set_tile((Ls2DTileMap *)self->tilemap,
                                              0,
                                              x,
                                              y,
                                              (Ls2DTile){ .gid = gids[index] });
                        ls2d_tilemap_set_tile((Ls2DTileMap *)self->tilemap,
                                              1,
                                              x,
                                              y,
                                              (Ls2DTile){ .gid = gids_2[index] });
                }
        }

        tsx = ls2d_tile_sheet_new_from_tsx(cache, "data/Overworld.tsx");
        ls2d_tilemap_add_tilesheet((Ls2DTileMap *)self->tilemap, tsx);
        ls2d_scene_add_entity(self->scene, self->tilemap);

        return true;
}
