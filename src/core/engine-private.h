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

#pragma once

#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "engine.h"
#include "scene.h"

/** Private API headers for the engine implementation */

/**
 * Ls2DEngine is responsible for managing the primary output, setting up
 * the event dispatch system, etc.
 */
struct Ls2DEngine {
        Ls2DObject object; /*< Parent */
        int width;
        int height;
        uint32_t fps_delay;
        SDL_Window *window;
        SDL_Renderer *render;
        bool running;
        bool fullscreen;

        /* List of scenes. */
        LsList *scenes;
        Ls2DScene *active_scene;
};

/**
 * Process all incoming events to the engine (input/updates)
 */
void ls2d_engine_process_events(Ls2DEngine *self, Ls2DFrameInfo *frame);

/**
 * Pump any and all drawing events
 */
void ls2d_engine_draw(Ls2DEngine *self, Ls2DFrameInfo *frame);

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
