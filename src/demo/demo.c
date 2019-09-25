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

static void add_player(Ls2DScene *scene, Ls2DTextureHandle handle)
{
        autofree(Ls2DEntity) *entity = NULL;
        autofree(Ls2DComponent) *sprite = NULL;

        entity = ls2d_entity_new("player");
        if (!entity) {
                exit(1);
        }
        sprite = ls2d_sprite_component_new();
        if (!sprite) {
                exit(1);
        }

        ls2d_sprite_component_set_texture((Ls2DSpriteComponent *)sprite, handle);
        ls2d_entity_add_component(entity, sprite);
        ls2d_scene_add_entity(scene, entity);
}

/**
 * Main entry point to the demo.
 */
int main(__ls_unused__ int argc, __ls_unused__ char **argv)
{
        autofree(Ls2DEngine) *engine = NULL;
        autofree(Ls2DScene) *scene = NULL;
        Ls2DTextureCache *cache = NULL;
        Ls2DTextureHandle handle;
        Ls2DTextureHandle handle2;
        Ls2DTextureHandle handle3;

        /* Construct new engine */
        engine = ls2d_engine_new_current_display();
        if (!engine) {
                return EXIT_FAILURE;
        }
        ls2d_engine_set_fps_cap(engine, 60);

        /* Create root scene */
        scene = ls2d_scene_new("game_screen");
        ls2d_engine_add_scene(engine, scene);

        cache = ls2d_scene_get_texture_cache(scene);
        handle = ls2d_texture_cache_load_file(cache,
                                              "demo_data/PNG/Sprites X2/Ships/spaceShips_006.png");
        handle2 = ls2d_texture_cache_load_file(cache,
                                               "demo_data/PNG/Sprites X2/Ships/spaceShips_004.png");

        handle3 = ls2d_texture_cache_load_file(cache,
                                               "demo_data/PNG/Sprites X2/Ships/spaceShips_007.png");

        for (int i = 0; i < 5; i++) {
                add_player(scene, handle);
        }

        for (int i = 0; i < 5; i++) {
                add_player(scene, handle2);
        }

        for (int i = 0; i < 5; i++) {
                add_player(scene, handle3);
        }

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
