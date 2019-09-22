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

#include "libls.h"
#include "object.h"
#include "sprite.h"

typedef struct Ls2DScene Ls2DScene;

/**
 * Construct a new scene object. Every scene contains a list of renderables
 * and will process input accordingly.
 * Notably, this optimises the event queue as we'll only dispatch the
 * input handling to the visible renderables within the active scene.
 */
Ls2DScene *ls2d_scene_new(const char *name);

/**
 * Unref an allocated Scene. This will also deference any
 * attached resources.
 */
Ls2DScene *ls2d_scene_unref(Ls2DScene *self);

/**
 * Get the name of this scene object
 */
const char *ls2d_scene_get_name(Ls2DScene *self);

/**
 * Attach the sprite to the this scene.
 * We'll increase the refcount here to retain a link to it.
 */
void ls2d_scene_add_sprite(Ls2DScene *self, Ls2DSprite *sprite);

DEF_AUTOFREE(Ls2DScene, ls2d_scene_unref)

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
