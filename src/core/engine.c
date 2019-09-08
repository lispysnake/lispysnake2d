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

#include "engine-private.h"
#include "ls2d.h"

static bool did_init_sdl = false;
static bool sdl_init(void);
static void sdl_deinit(void);

/**
 * Helper to get a useful framerate limit.
 */
static inline uint16_t ls2d_get_framerate(void)
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

        return mode.refresh_rate;

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

        engine = calloc(1, sizeof(struct Ls2DEngine));
        if (!engine) {
                return NULL;
        }

        engine->width = width;
        engine->height = height;
        engine->running = false;
        engine->fps_delay = 0; /*< TODO: Autoset. */

        engine->fps_delay = ls2d_get_framerate();

        /* Setup the window */
        engine->window = SDL_CreateWindow("lispysnake2d",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          width,
                                          height,
                                          SDL_WINDOW_HIDDEN);
        if (!engine->window) {
                SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO,
                                "Couldn't create window: %s",
                                SDL_GetError());
                ls2d_engine_free(engine);
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
                ls2d_engine_free(engine);
                return NULL;
        }

        return engine;
}

Ls2DEngine *ls2d_engine_new_current_display()
{
        SDL_DisplayMode mode = { 0 };
        SDL_Rect area = { 0, 0, 320, 240 };

        if (!sdl_init()) {
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

void ls2d_engine_free(Ls2DEngine *self)
{
        if (!self) {
                goto cleanup;
        }
        if (self->render) {
                SDL_DestroyRenderer(self->render);
        }
        if (self->window) {
                SDL_DestroyWindow(self->window);
        }
        free(self);

cleanup:
        sdl_deinit();
}

/**
 * Internally we're responsible for the primary event queue, so we'll
 * manage it here and if necessary dispatch it.
 */
bool ls2d_engine_run(Ls2DEngine *self)
{
        uint32_t tick_start = 0;
        Ls2DFrameInfo frame = { 0 };

        /* Make sure to show the window */
        SDL_ShowWindow(self->window);

        /* Get the ball rolling */
        self->running = true;
        tick_start = SDL_GetTicks();
        frame.prev_ticks = tick_start - 1;

        /* Primary event loop */
        while (self->running) {
                /* Update frameinfo */
                frame.ticks = SDL_GetTicks() - tick_start;

                ls2d_engine_process_events(self, &frame);
                ls2d_engine_draw(self, &frame);

                /* Stash ticks */
                uint32_t tick_delay = frame.ticks - frame.prev_ticks;
                frame.prev_ticks = frame.ticks;

                /* If framerate cap is set, use it. */
                if (self->fps_delay > 0 && tick_delay < 1000 / self->fps_delay) {
                        SDL_Delay((1000 / self->fps_delay) - (tick_delay));
                }
        }

        return true;
}

void ls2d_engine_process_events(Ls2DEngine *self, Ls2DFrameInfo *frame)
{
        SDL_Event event = { 0 };

        /* Event update */
        while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_QUIT) {
                        self->running = false;
                }
        }
}

void ls2d_engine_draw(Ls2DEngine *self, Ls2DFrameInfo *frame)
{
        /* Render update */
        SDL_SetRenderDrawColor(self->render, 169, 203, 152, 255);
        SDL_RenderClear(self->render);
        SDL_RenderPresent(self->render);
}

void ls2d_engine_set_fullscreen(Ls2DEngine *self, bool fullscreen)
{
        if (!self || !self->window) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ls2DEngine not yet initialised");
                return;
        }
        SDL_SetWindowFullscreen(self->window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void ls2d_engine_set_fps_cap(Ls2DEngine *self, int16_t fps)
{
        if (!self) {
                return;
        }
        self->fps_delay = fps;
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
