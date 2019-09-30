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
 * Constructs a new Ls2DAnimation object.
 */
Ls2DAnimation *ls2d_animation_new(void);

/**
 * Unrefs a previously allocated animation object.
 */
Ls2DAnimation *ls2d_animation_unref(Ls2DAnimation *self);

/**
 * Add a frame to the animation. Duration is given in ms.
 */
bool ls2d_animation_add_frame(Ls2DAnimation *self, Ls2DTextureHandle handle, uint32_t duration);

/**
 * Update the animation. This should only be called by the owning objects.
 */
void ls2d_animation_update(Ls2DAnimation *self, Ls2DFrameInfo *frame);

/**
 * Set the animation to loop.
 * Typically we loop by default.
 */
void ls2d_animation_set_looping(Ls2DAnimation *self, bool looping);

/**
 * Reset to first frame
 */
void ls2d_animation_reset(Ls2DAnimation *self);

Ls2DTextureHandle ls2d_animation_get_texture(Ls2DAnimation *self);

/**
 * Stop the animation.
 */
void ls2d_animation_stop(Ls2DAnimation *self);

DEF_AUTOFREE(Ls2DAnimation, ls2d_animation_unref)

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
