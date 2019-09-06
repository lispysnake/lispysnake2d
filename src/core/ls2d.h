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

#include <stdbool.h>

/**
 * The Ls2DEngine is responsible for core lifecycle management.
 */
typedef struct Ls2DEngine Ls2DEngine;

/**
 * Return a new Ls2DEngine object
 */
Ls2DEngine *ls2d_engine_new(int width, int height);

/**
 * Return a new Ls2DEngine object for the current display size
 */
Ls2DEngine *ls2d_engine_new_current_display(void);

/**
 * Destroy a previously allocated Ls2DEngine object
 */
void ls2d_engine_free(Ls2DEngine *self);

/**
 * Run an Ls2DEngine until termination.
 */
bool ls2d_engine_run(Ls2DEngine *self);

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
