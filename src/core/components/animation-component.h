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

#include "ls2d.h"

/**
 * ls2d_animation_component adds drawable behaviours to an Ls2DBasicEntity.
 */
typedef struct Ls2DAnimationComponent Ls2DAnimationComponent;

/**
 * Construct a new AnimationComponent object.
 */
Ls2DComponent *ls2d_animation_component_new(void);

/**
 * Unref an allocated AnimationComponent. This will also deference any
 * attached resources.
 */
Ls2DAnimationComponent *ls2d_animation_component_unref(Ls2DAnimationComponent *self);

/**
 * Insert an animation into this component by a string ID.
 * We can then activate animations by their name.
 */
bool ls2d_animation_component_add_animation(Ls2DAnimationComponent *self, const char *id,
                                            Ls2DAnimation *animation);

/**
 * Set the current animation
 */
bool ls2d_animation_component_set_animation(Ls2DAnimationComponent *self, const char *id);

/**
 * Return the currently renderable animation frame
 */
Ls2DTextureHandle ls2d_animation_component_get_texture(Ls2DAnimationComponent *self);

DEF_AUTOFREE(Ls2DAnimationComponent, ls2d_animation_component_unref)

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
