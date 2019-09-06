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

#include "ls2d.h"

static bool did_init_sdl = false;
static bool sdl_init(void);
static void sdl_deinit(void);

/**
 * Ls2DEngine is responsible for managing the primary output, setting up
 * the event dispatch system, etc.
 */
struct Ls2DEngine {
        int width;
        int height;
        SDL_Window *window;
        SDL_Renderer *render;
        bool running;
};

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
        SDL_Event event = { 0 };

        self->running = true;

        /* Make sure to show he window */
        SDL_ShowWindow(self->window);

        /* TODO: Split to non-polling event and render-cycle */
        while (self->running) {
                while (SDL_PollEvent(&event) != 0) {
                        if (event.type == SDL_QUIT) {
                                self->running = false;
                        }
                }

                /* TODO: Render */
                SDL_SetRenderDrawColor(self->render, 169, 203, 152, 255);
                SDL_RenderClear(self->render);
                SDL_RenderPresent(self->render);
        }

        return true;
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
