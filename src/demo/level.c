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

bool demo_game_load_tilemap(DemoGame *self)
{
        Ls2DTextureCache *cache = ls2d_scene_get_texture_cache(self->scene);
        autofree(Ls2DTileSheet) *tsx = ls2d_tile_sheet_new_from_tsx(cache, "data/Overworld.tsx");

        /*
        self->tilemap = ls2d_tilemap_new(70, 70, 20);
        self->tilemap = ls2d_tilemap_new_from_tmx("data/level1.tmx");
        ls2d_scene_add_entity(self->scene, self->tilemap);
        * */

        return true;
}
