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
        int __reserved1;
        int width;
        int height;
        SDL_Window *window;
};

Ls2DEngine *ls2d_engine_new(int width, int height)
{
        Ls2DEngine *engine = NULL;

        if (!sdl_init()) {
                return NULL;
        }

        engine = calloc(1, sizeof(engine));
        if (!engine) {
                return NULL;
        }

        /* TODO: Init the engine.. */
        engine->width = width;
        engine->height = height;
        engine->window = SDL_CreateWindow("lispysnake2d",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          width,
                                          height,
                                          SDL_WINDOW_HIDDEN);
        return engine;
}

Ls2DEngine *ls2d_engine_new_current_display()
{
        SDL_DisplayMode mode = { 0 };
        SDL_Rect area = { 0, 0, 320, 240 };

        if (SDL_GetCurrentDisplayMode(0, &mode) == 0) {
                area.w = mode.w;
                area.h = mode.h;
        }

        return ls2d_engine_new(area.w, area.h);
}

void ls2d_engine_free(Ls2DEngine *self)
{
        if (!self) {
                goto cleanup;
        }
        if (self->window) {
                SDL_DestroyWindow(self->window);
        }
        free(self);

cleanup:
        sdl_deinit();
}

bool ls2d_engine_run(Ls2DEngine *self)
{
        return false;
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

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
