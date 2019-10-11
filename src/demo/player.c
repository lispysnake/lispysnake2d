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

static Ls2DAnimation *demo_player_animation(DemoGame *self)
{
        autofree(Ls2DTileSheet) *sheet = NULL;
        Ls2DAnimation *walking = NULL;
        Ls2DTextureCache *cache = NULL;

        /* Grab our textures */
        cache = ls2d_scene_get_texture_cache(self->scene);
        sheet = ls2d_tile_sheet_new_from_xml(cache, "demo_data/platform/spritesheet_player1.xml");

        walking = ls2d_animation_new();
        ls2d_animation_set_looping(walking, true);
        uint32_t duration = 1000 / 20;
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_get(sheet, "p1_walk01.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_get(sheet, "p1_walk02.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_get(sheet, "p1_walk03.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_get(sheet, "p1_walk04.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_get(sheet, "p1_walk05.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_get(sheet, "p1_walk06.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_get(sheet, "p1_walk07.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_get(sheet, "p1_walk08.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_get(sheet, "p1_walk09.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_get(sheet, "p1_walk10.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_get(sheet, "p1_walk11.png"), duration);

        return walking;
}

bool demo_game_make_player(DemoGame *self)
{
        autofree(Ls2DComponent) *sprite = NULL;
        autofree(Ls2DComponent) *pos = NULL;
        autofree(Ls2DComponent) *anim = NULL;
        autofree(Ls2DAnimation) *animation = NULL;

        self->player = ls2d_basic_entity_new("player");
        sprite = ls2d_sprite_component_new();
        pos = ls2d_position_component_new();
        anim = ls2d_animation_component_new();

        animation = demo_player_animation(self);
        ls2d_animation_component_add_animation((Ls2DAnimationComponent *)anim,
                                               "walking",
                                               animation);
        ls2d_entity_add_component(self->player, sprite);
        ls2d_entity_add_component(self->player, pos);
        ls2d_entity_add_component(self->player, anim);
        ls2d_sprite_component_set_flip((Ls2DSpriteComponent *)sprite, SDL_FLIP_HORIZONTAL);
        ls2d_position_component_set_xy((Ls2DPositionComponent *)pos,
                                       (SDL_Point){ .x = 690, .y = 16 * 70 - 25 });
        ls2d_scene_add_entity(self->scene, self->player);

        return true;
}
