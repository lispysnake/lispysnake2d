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

#include <SDL.h>
#include <stdlib.h>

#include "libls.h"
#include "ls2d.h"

/**
 * Return an entity that will become the main player.
 */
static Ls2DEntity *demo_add_player(Ls2DScene *scene, Ls2DTextureHandle handle,
                                   Ls2DAnimation *animation)
{
        Ls2DEntity *entity = NULL;
        autofree(Ls2DComponent) *sprite = NULL;
        autofree(Ls2DComponent) *pos = NULL;
        autofree(Ls2DComponent) *anim = NULL;

        entity = ls2d_entity_new("player");
        if (!entity) {
                return NULL;
        }
        sprite = ls2d_sprite_component_new();
        if (!sprite) {
                return NULL;
        }
        pos = ls2d_position_component_new();
        if (!pos) {
                return NULL;
        }
        anim = ls2d_animation_component_new();
        if (!anim) {
                return NULL;
        }
        ls2d_animation_component_add_animation((Ls2DAnimationComponent *)anim,
                                               "walking",
                                               animation);
        ls2d_sprite_component_set_texture((Ls2DSpriteComponent *)sprite, handle);
        ls2d_entity_add_component(entity, sprite);
        ls2d_entity_add_component(entity, pos);
        ls2d_entity_add_component(entity, anim);
        ls2d_position_component_set_xy((Ls2DPositionComponent *)pos,
                                       (SDL_Point){ .x = 300, .y = 300 });
        ls2d_scene_add_entity(scene, entity);

        return entity;
}

/**
 * Main entry point to the demo.
 */
int main(__ls_unused__ int argc, __ls_unused__ char **argv)
{
        autofree(Ls2DEngine) *engine = NULL;
        autofree(Ls2DScene) *scene = NULL;
        autofree(Ls2DEntity) *player = NULL;
        autofree(Ls2DTileSheet) *sheet = NULL;
        autofree(Ls2DAnimation) *walking = NULL;
        Ls2DTextureCache *cache = NULL;

        /* Construct new engine */
        engine = ls2d_engine_new(800, 600);
        if (!engine) {
                return EXIT_FAILURE;
        }
        ls2d_engine_set_fps_cap(engine, 60);

        /* Create root scene */
        scene = ls2d_scene_new("game_screen");
        ls2d_engine_add_scene(engine, scene);

        /* Grab our textures */
        cache = ls2d_scene_get_texture_cache(scene);
        sheet = ls2d_tile_sheet_new_from_xml(cache, "demo_data/platform/spritesheet_player1.xml");

        walking = ls2d_animation_new();
        ls2d_animation_set_looping(walking, true);
        const uint32_t duration = 1000 / 20;
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_lookup(sheet, "p1_walk01.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_lookup(sheet, "p1_walk02.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_lookup(sheet, "p1_walk03.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_lookup(sheet, "p1_walk04.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_lookup(sheet, "p1_walk05.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_lookup(sheet, "p1_walk06.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_lookup(sheet, "p1_walk07.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_lookup(sheet, "p1_walk08.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_lookup(sheet, "p1_walk09.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_lookup(sheet, "p1_walk10.png"), duration);
        ls2d_animation_add_frame(walking, ls2d_tile_sheet_lookup(sheet, "p1_walk11.png"), duration);

        /* Sort out our player */
        player = demo_add_player(scene, ls2d_tile_sheet_lookup(sheet, "p1_stand.png"), walking);

        int ret = ls2d_engine_run(engine);
        return ret;
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
