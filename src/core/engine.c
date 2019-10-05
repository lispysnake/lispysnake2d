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

#define _GNU_SOURCE

#include <SDL.h>
#include <libxml/parser.h>
#include <libxml/xmlversion.h>
#include <stdlib.h>

#include "engine-private.h"

static bool did_init_xml = false;
static bool did_init_sdl = false;
static bool sdl_init(void);
static void sdl_deinit(void);
static bool xml_init(void);
static void xml_deinit(void);
static void ls2d_engine_destroy(Ls2DEngine *self);

/**
 * Main vtable for our engine
 */
static Ls2DObjectTable engine_vtable = {
        .destroy = (ls2d_object_vfunc_destroy)ls2d_engine_destroy,
        .obj_name = "Ls2DEngine",
};

/**
 * Helper to get a useful framerate limit.
 */
static inline uint32_t ls2d_get_framerate(void)
{
        SDL_DisplayMode mode = { 0 };

        if (SDL_GetCurrentDisplayMode(0, &mode) != 0) {
                goto fail;
        }

        SDL_Log("Refresh rate discovered: %d", mode.refresh_rate);
        if (mode.refresh_rate < 50) {
                SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Slow limit framerate, enforcing 60fps");
                return 60;
        }

        return (uint32_t)mode.refresh_rate;

fail:
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to get refresh rate: %s", SDL_GetError());
        return 0;
}

Ls2DEngine *ls2d_engine_new(int width, int height)
{
        Ls2DEngine *engine = NULL;

        if (!sdl_init()) {
                return NULL;
        }
        if (!xml_init()) {
                return NULL;
        }

        engine = calloc(1, sizeof(struct Ls2DEngine));
        if (!engine) {
                return NULL;
        }

        engine->width = width;
        engine->height = height;
        engine->running = false;
        engine->fullscreen = false;

        /* autoset fps cap */
        engine->fps_delay = ls2d_get_framerate();

        /* Setup the window */
        engine->window = SDL_CreateWindow("lispysnake2d",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          width,
                                          height,
                                          SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI);
        if (!engine->window) {
                SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO,
                                "Couldn't create window: %s",
                                SDL_GetError());
                ls2d_engine_destroy(engine);
                return NULL;
        }

        /* Setup our renderer, lock vsync for now. */
        engine->render = SDL_CreateRenderer(engine->window,
                                            -1,
                                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!engine->render) {
                SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO,
                                "Couldn't create a renderer: %s",
                                SDL_GetError());
                ls2d_engine_destroy(engine);
                return NULL;
        }
        SDL_RenderSetLogicalSize(engine->render, width, height);

        engine->buffer = SDL_CreateTexture(engine->render,
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET,
                                           width,
                                           height);
        if (ls_unlikely(!engine->buffer)) {
                SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO,
                                "Couldn't create double buffer: %s\n",
                                SDL_GetError());
                ls2d_engine_destroy(engine);
                return NULL;
        }

        engine->input_manager = ls2d_input_manager_new();
        if (!engine->input_manager) {
                SDL_LogCritical(SDL_LOG_CATEGORY_INPUT, "Couldn't create InputManager");
                ls2d_engine_destroy(engine);
                return NULL;
        }

        return ls2d_object_init((Ls2DObject *)engine, &engine_vtable);
}

Ls2DEngine *ls2d_engine_new_current_display()
{
        SDL_DisplayMode mode = { 0 };
        SDL_Rect area = { 0, 0, 320, 240 };

        if (!sdl_init()) {
                return NULL;
        }
        if (!xml_init()) {
                return NULL;
        }

        /* Fetch current resolution, or fallback to busted default. */
        if (SDL_GetCurrentDisplayMode(0, &mode) == 0) {
                area.w = mode.w;
                area.h = mode.h;
        } else {
                SDL_LogError(SDL_LOG_CATEGORY_VIDEO,
                             "Failed to fetch display mode: %s",
                             SDL_GetError());
        }

        return ls2d_engine_new(area.w, area.h);
}

Ls2DEngine *ls2d_engine_unref(Ls2DEngine *self)
{
        return ls2d_object_unref(self);
}

static inline void free_scene(void *v)
{
        (void)ls2d_scene_unref(v);
}

static void ls2d_engine_destroy(Ls2DEngine *self)
{
        if (ls_unlikely(!self)) {
                goto cleanup;
        }
        if (ls_likely(self->game != NULL) && ls_likely(self->game->funcs.destroy != NULL)) {
                self->game->funcs.destroy(self->game);
        }
        if (ls_likely(self->render != NULL)) {
                SDL_DestroyRenderer(self->render);
        }
        if (ls_likely(self->window != NULL)) {
                SDL_DestroyWindow(self->window);
        }
        if (ls_likely(self->scenes != NULL)) {
                ls_list_free_full(self->scenes, free_scene);
        }
        if (ls_likely(self->input_manager != NULL)) {
                ls2d_input_manager_unref(self->input_manager);
        }
        free(self);

cleanup:
        sdl_deinit();
        xml_deinit();
}

/**
 * Internally we're responsible for the primary event queue, so we'll
 * manage it here and if necessary dispatch it.
 */
int ls2d_engine_run(Ls2DEngine *self, Ls2DGame *game)
{
        Ls2DFrameInfo frame = { 0 };
        if (ls_unlikely(!self)) {
                return EXIT_FAILURE;
        }
        if (ls_unlikely(!game)) {
                fprintf(stderr, "Missing game!\n");
                return EXIT_FAILURE;
        }

        game->engine = self;
        self->game = game;

        /* Make sure to show the window */
        SDL_ShowWindow(self->window);

        /* Get the ball rolling */
        self->running = true;
        ls2d_frame_info_init(&frame);

        frame.window = self->window;
        frame.renderer = self->render;

        if (game->funcs.init) {
                if (!game->funcs.init(game)) {
                        fprintf(stderr, "Game init failed\n");
                        return EXIT_FAILURE;
                }
        }

        /* Primary event loop */
        while (self->running) {
                /* Update frameinfo */
                ls2d_frame_info_tick(&frame);

                ls2d_engine_process_events(self, &frame);
                ls2d_engine_draw(self, &frame);

                /* Stash ticks */
                ls2d_frame_info_stash(&frame);

                double fps = ls2d_frame_info_get_fps(&frame);
                char *s = NULL;
                asprintf(&s, "demo: %.1f/fps", fps);
                SDL_SetWindowTitle(frame.window, s);
                free(s);

                /* If framerate cap is set, use it.
                ls2d_frame_info_delay(&frame, self->fps_delay, fps);*/
        }

        return EXIT_SUCCESS;
}

void ls2d_engine_set_fullscreen(Ls2DEngine *self, bool fullscreen)
{
        if (!self || !self->window) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ls2DEngine not yet initialised");
                return;
        }
        self->fullscreen = fullscreen;
        SDL_SetWindowFullscreen(self->window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void ls2d_engine_set_fps_cap(Ls2DEngine *self, uint32_t fps)
{
        if (ls_unlikely(!self)) {
                return;
        }
        self->fps_delay = fps;
}

void ls2d_engine_add_scene(Ls2DEngine *self, Ls2DScene *scene)
{
        if (ls_unlikely(!self) || ls_unlikely(!scene)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ls2DEngine not yet initialised");
                return;
        }
        self->scenes = ls_list_append(self->scenes, ls2d_object_ref(scene));
        if (!self->active_scene) {
                self->active_scene = scene;
        }
}

Ls2DInputManager *ls2d_engine_get_input_manager(Ls2DEngine *self)
{
        if (ls_unlikely(!self)) {
                return NULL;
        }
        return self->input_manager;
}

/**
 * Ensure we have initialisation of the correct subsystems
 * Note this is kinda nasty but it saves calling it from our
 * application.
 */
static bool sdl_init()
{
        if (did_init_sdl) {
                return true;
        }

        did_init_sdl = true;

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
                return false;
        }

        return true;
}

/**
 * Ensure we unload SDL when the Engine is destroyed. Again this
 * is kinda nasty, and not appropriate for most libraries, but it fits
 * in with the single-engine-game approach we need.
 */
static void sdl_deinit()
{
        SDL_Quit();
        did_init_sdl = false;
}

static bool xml_init()
{
        if (did_init_xml) {
                return true;
        }
        did_init_xml = true;

        LIBXML_TEST_VERSION

        return true;
}

static void xml_deinit()
{
        if (!did_init_xml) {
                return;
        }
        xmlCleanupParser();
        did_init_xml = false;
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
