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
        ls2d_scene_add_entity(scene, entity);

        return entity;
}

static bool mouse_button_callback(SDL_MouseButtonEvent *event, Ls2DFrameInfo *frame, void *userdata)
{
        fprintf(stderr, "Clicked at %d %d!\n", event->x, event->y);
        return false;
}

static bool mouse_motion_callback(SDL_MouseMotionEvent *event, Ls2DFrameInfo *frame, void *userdata)
{
        Ls2DEntity *ent = userdata;
        Ls2DPositionComponent *pos =
            (Ls2DPositionComponent *)ls2d_entity_get_component(ent, LS2D_COMP_ID_POSITION);
        ls2d_position_component_set_xy(pos, (SDL_Point){ .x = event->x, .y = event->y });

        return false;
}

/**
 * Main entry point to the demo.
 */
int main(__ls_unused__ int argc, __ls_unused__ char **argv)
{
        autofree(Ls2DEngine) *engine = NULL;
        autofree(Ls2DScene) *scene = NULL;
        autofree(Ls2DEntity) *player = NULL;
        Ls2DTextureCache *cache = NULL;
        Ls2DTextureHandle handle;
        Ls2DTextureHandle subhandle;
        Ls2DInputManager *imanager = NULL;

        /* Construct new engine */
        engine = ls2d_engine_new_current_display();
        if (!engine) {
                return EXIT_FAILURE;
        }
        ls2d_engine_set_fps_cap(engine, 1);

        imanager = ls2d_engine_get_input_manager(engine);

        /* Create root scene */
        scene = ls2d_scene_new("game_screen");
        ls2d_engine_add_scene(engine, scene);

        cache = ls2d_scene_get_texture_cache(scene);
        handle =
            ls2d_texture_cache_load_file(cache,
                                         "demo_data/Spritesheet/spaceShooter2_spritesheet_2X.png");
        subhandle =
            ls2d_texture_cache_subregion(cache,
                                         handle,
                                         (SDL_Rect){ .x = 896, .y = 305, .w = 228, .h = 163 });

        /* Sort out our player */
        player = demo_add_player(scene, subhandle);

        ls2d_input_manager_set_mouse_button_callback(imanager, mouse_button_callback, player);
        ls2d_input_manager_set_mouse_motion_callback(imanager, mouse_motion_callback, player);

        return ls2d_engine_run(engine);
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
