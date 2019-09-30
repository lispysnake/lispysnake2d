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

static Ls2DTextureHandle handles[11];
static int animation_frame = 0;
static int n_frames = LS_ARRAY_SIZE(handles);

/**
 * Return an entity that will become the main player.
 */
static Ls2DEntity *demo_add_player(Ls2DScene *scene, Ls2DTextureHandle handle)
{
        Ls2DEntity *entity = NULL;
        autofree(Ls2DComponent) *sprite = NULL;
        autofree(Ls2DComponent) *pos = NULL;

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
        ls2d_sprite_component_set_texture((Ls2DSpriteComponent *)sprite, handle);
        ls2d_entity_add_component(entity, sprite);
        ls2d_entity_add_component(entity, pos);
        ls2d_position_component_set_xy((Ls2DPositionComponent *)pos,
                                       (SDL_Point){ .x = 300, .y = 300 });
        ls2d_scene_add_entity(scene, entity);

        return entity;
}

static bool my_callback(SDL_MouseButtonEvent *event, Ls2DFrameInfo *frame, void *userdata)
{
        Ls2DEntity *player = userdata;
        Ls2DComponent *sprite = ls2d_entity_get_component(player, LS2D_COMP_ID_SPRITE);
        ls2d_sprite_component_set_texture((Ls2DSpriteComponent *)sprite,
                                          handles[animation_frame % n_frames]);
        animation_frame++;
        return true;
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
        Ls2DInputManager *imanager = NULL;

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

        handles[0] = ls2d_tile_sheet_lookup(sheet, "p1_walk01.png");
        handles[1] = ls2d_tile_sheet_lookup(sheet, "p1_walk02.png");
        handles[2] = ls2d_tile_sheet_lookup(sheet, "p1_walk03.png");
        handles[3] = ls2d_tile_sheet_lookup(sheet, "p1_walk04.png");
        handles[4] = ls2d_tile_sheet_lookup(sheet, "p1_walk05.png");
        handles[5] = ls2d_tile_sheet_lookup(sheet, "p1_walk06.png");
        handles[6] = ls2d_tile_sheet_lookup(sheet, "p1_walk07.png");
        handles[7] = ls2d_tile_sheet_lookup(sheet, "p1_walk08.png");
        handles[8] = ls2d_tile_sheet_lookup(sheet, "p1_walk09.png");
        handles[9] = ls2d_tile_sheet_lookup(sheet, "p1_walk10.png");
        handles[10] = ls2d_tile_sheet_lookup(sheet, "p1_walk11.png");

        /* Sort out our player */
        player = demo_add_player(scene, handles[0]);

        imanager = ls2d_engine_get_input_manager(engine);
        ls2d_input_manager_set_mouse_button_callback(imanager, my_callback, player);

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
