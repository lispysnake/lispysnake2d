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

        entity = ls2d_basic_entity_new("player");
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
                                       (SDL_Point){ .x = 40, .y = 150 });
        ls2d_scene_add_entity(scene, entity);

        return entity;
}

/**
 * Return an entity that will become an NPC
 */
static Ls2DEntity *demo_add_npc(Ls2DScene *scene, Ls2DTextureHandle handle,
                                Ls2DAnimation *animation)
{
        Ls2DEntity *entity = NULL;
        autofree(Ls2DComponent) *sprite = NULL;
        autofree(Ls2DComponent) *pos = NULL;
        autofree(Ls2DComponent) *anim = NULL;

        entity = ls2d_basic_entity_new("npc");
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
                                       (SDL_Point){ .x = 500, .y = 50 });
        ls2d_scene_add_entity(scene, entity);

        return entity;
}

static Ls2DEntity *add_tilemap(Ls2DScene *scene)
{
        Ls2DEntity *entity = NULL;

        entity = ls2d_tilemap_new(70, 10, 10);
        if (!entity) {
                abort();
                return NULL;
        }

        ls2d_scene_add_entity(scene, entity);

        /* TODO: Actually populate the tilemap! */
        if (!ls2d_tilemap_set_tile((Ls2DTileMap *)entity,
                                   0,
                                   0,
                                   0,
                                   (Ls2DTile){ .gid = 12, .flipped_horizontal = true })) {
                abort();
        }
        ls2d_tilemap_set_tile((Ls2DTileMap *)entity, 0, 2, 0, (Ls2DTile){ .gid = 12 });
        ls2d_tilemap_set_tile((Ls2DTileMap *)entity,
                              0,
                              2,
                              0,
                              (Ls2DTile){ .gid = 12, .flipped_diagonal = true });
        ls2d_tilemap_set_tile((Ls2DTileMap *)entity, 0, 3, 0, (Ls2DTile){ .gid = 12 });
        ls2d_tilemap_set_tile((Ls2DTileMap *)entity, 0, 4, 0, (Ls2DTile){ .gid = 13 });
        ls2d_tilemap_set_tile((Ls2DTileMap *)entity,
                              0,
                              4,
                              4,
                              (Ls2DTile){ .gid = 12, .flipped_vertical = true });
        ls2d_tilemap_set_tile((Ls2DTileMap *)entity, 0, 0, 4, (Ls2DTile){ .gid = 13 });
        ls2d_tilemap_set_tile((Ls2DTileMap *)entity, 0, 2, 2, (Ls2DTile){ .gid = 12 });
        ls2d_tilemap_set_tile((Ls2DTileMap *)entity, 0, 1, 4, (Ls2DTile){ .gid = 12 });
        ls2d_tilemap_set_tile((Ls2DTileMap *)entity, 0, 2, 4, (Ls2DTile){ .gid = 13 });
        ls2d_tilemap_set_tile((Ls2DTileMap *)entity, 0, 3, 4, (Ls2DTile){ .gid = 11 });
        ls2d_tilemap_set_tile((Ls2DTileMap *)entity, 0, 9, 9, (Ls2DTile){ .gid = 12 });
        return entity;
}

static bool mouse_callback(SDL_MouseMotionEvent *event, Ls2DFrameInfo *frame, void *userdata)
{
        Ls2DCamera *camera = userdata;
        ls2d_camera_set_xy(camera, (SDL_Point){ .x = event->x, .y = event->y });
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
        autofree(Ls2DEntity) *npc = NULL;
        autofree(Ls2DEntity) *tilemap = NULL;
        autofree(Ls2DTileSheet) *sheet = NULL;
        autofree(Ls2DTileSheet) *sheet_npc = NULL;
        autofree(Ls2DAnimation) *walking = NULL;
        autofree(Ls2DAnimation) *walking_npc = NULL;
        autofree(Ls2DCamera) *camera = NULL;
        Ls2DInputManager *imanager = NULL;

        SDL_Rect world_bounds = { 0 };
        world_bounds.w = 1024;
        world_bounds.h = 600;

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
        sheet_npc =
            ls2d_tile_sheet_new_from_xml(cache, "demo_data/platform/spritesheet_player3.xml");

        {
                autofree(Ls2DTileSheet) *t_sheet =
                    ls2d_tile_sheet_new_from_tsx(cache, "demo_data/TESTMAP.tsx");
        }

        walking = ls2d_animation_new();
        ls2d_animation_set_looping(walking, true);
        uint32_t duration = 1000 / 20;
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

        tilemap = add_tilemap(scene);

        /* Sort out our player */
        player = demo_add_player(scene, ls2d_tile_sheet_lookup(sheet, "p1_stand.png"), walking);

        walking_npc = ls2d_animation_new();
        duration = 1000 / 25;
        ls2d_animation_set_looping(walking_npc, true);
        ls2d_animation_add_frame(walking_npc,
                                 ls2d_tile_sheet_lookup(sheet_npc, "p3_walk01.png"),
                                 duration);
        ls2d_animation_add_frame(walking_npc,
                                 ls2d_tile_sheet_lookup(sheet_npc, "p3_walk02.png"),
                                 duration);
        ls2d_animation_add_frame(walking_npc,
                                 ls2d_tile_sheet_lookup(sheet_npc, "p3_walk03.png"),
                                 duration);
        ls2d_animation_add_frame(walking_npc,
                                 ls2d_tile_sheet_lookup(sheet_npc, "p3_walk04.png"),
                                 duration);
        ls2d_animation_add_frame(walking_npc,
                                 ls2d_tile_sheet_lookup(sheet_npc, "p3_walk05.png"),
                                 duration);
        ls2d_animation_add_frame(walking_npc,
                                 ls2d_tile_sheet_lookup(sheet_npc, "p3_walk06.png"),
                                 duration);
        ls2d_animation_add_frame(walking_npc,
                                 ls2d_tile_sheet_lookup(sheet_npc, "p3_walk07.png"),
                                 duration);
        ls2d_animation_add_frame(walking_npc,
                                 ls2d_tile_sheet_lookup(sheet_npc, "p3_walk08.png"),
                                 duration);
        ls2d_animation_add_frame(walking_npc,
                                 ls2d_tile_sheet_lookup(sheet_npc, "p3_walk09.png"),
                                 duration);
        ls2d_animation_add_frame(walking_npc,
                                 ls2d_tile_sheet_lookup(sheet_npc, "p3_walk10.png"),
                                 duration);
        ls2d_animation_add_frame(walking_npc,
                                 ls2d_tile_sheet_lookup(sheet_npc, "p3_walk11.png"),
                                 duration);

        npc = demo_add_npc(scene, ls2d_tile_sheet_lookup(sheet_npc, "p3_stand.png"), walking_npc);

        camera = ls2d_camera_new(scene);
        ls2d_camera_set_world_bounds(camera, world_bounds);
        ls2d_camera_set_xy(camera, (SDL_Point){ .x = 0, .y = 500 });
        ls2d_scene_add_camera(scene, "primary", camera);

        imanager = ls2d_engine_get_input_manager(engine);
        ls2d_input_manager_set_mouse_motion_callback(imanager, mouse_callback, camera);
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
