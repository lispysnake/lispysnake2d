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

static bool mouse_callback(SDL_MouseMotionEvent *event, Ls2DFrameInfo *frame, void *userdata)
{
        Ls2DCamera *camera = userdata;
        ls2d_camera_set_xy(camera, (SDL_Point){ .x = event->x, .y = event->y });
        return true;
}

bool demo_game_init(Ls2DGame *game)
{
        DemoGame *self = (DemoGame *)game;
        Ls2DInputManager *imanager = NULL;

        SDL_Rect world_bounds = { 0 };
        world_bounds.w = 45 * 16;
        world_bounds.h = 25 * 16;

        fprintf(stderr, "Init begin!\n");

        /* Construct scene */
        self->scene = ls2d_scene_new();
        imanager = ls2d_engine_get_input_manager(game->engine);

        /* Camera */
        self->camera = ls2d_camera_new(self->scene);
        ls2d_scene_add_camera(self->scene, "main_camera", self->camera);
        ls2d_camera_set_world_bounds(self->camera, world_bounds);

        /* Tilesheet */
        if (!demo_game_load_tilemap(self)) {
                return false;
        }
        /* Player
        if (!demo_game_make_player(self)) {
                return false;
        }*/

        ls2d_input_manager_set_mouse_motion_callback(imanager, mouse_callback, self->camera);

        ls2d_engine_add_scene(game->engine, self->scene);
        fprintf(stderr, "Init end!\n");

        return true;
}

void demo_game_destroy(Ls2DGame *game)
{
        DemoGame *self = (DemoGame *)game;
        fprintf(stderr, "Destroy begin!\n");

        /* Cleanup allocations */
        ls2d_scene_unref(self->scene);
        ls2d_camera_unref(self->camera);
        ls2d_tilemap_unref(self->tilemap);

        fprintf(stderr, "Destroy end!\n");
}

int main(int argc, char **argv)
{
        autofree(Ls2DEngine) *engine = NULL;
        DemoGame game = { 0 };
        game.parent.funcs.init = demo_game_init;
        game.parent.funcs.destroy = demo_game_destroy;

        engine = ls2d_engine_new(480, 270);
        return ls2d_engine_run(engine, (Ls2DGame *)&game);
}
