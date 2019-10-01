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

#include "ls2d.h"

/**
 * Construct a new Ls2DCamera. This allows manipulation of the view
 * within a scene. Without a camera, all scenes are static.
 */
Ls2DCamera *ls2d_camera_new(Ls2DScene *scene);

/**
 * Unref a previously allocated camera
 */
Ls2DCamera *ls2d_camera_unref(Ls2DCamera *self);

/**
 * Set the bounds for the camera.
 */
void ls2d_camera_set_world_bounds(Ls2DCamera *self, SDL_Rect bounds);

/**
 * Set the X,Y for the camera to look at.
 * This function will automatically adjust to the world bounds.
 */
void ls2d_camera_set_xy(Ls2DCamera *self, SDL_Point lookat);

/**
 * Returns true if the entity is within the bounds of the camera.
 */
bool ls2d_camera_entity_in_bounds(Ls2DCamera *self, Ls2DBasicEntity *entity);

bool ls2d_camera_convert_entity_position(Ls2DCamera *self, Ls2DBasicEntity *entity, int *x, int *y);

/**
 * Update the camera
 */
void ls2d_camera_update(Ls2DCamera *self, Ls2DFrameInfo *frame);

DEF_AUTOFREE(Ls2DCamera, ls2d_camera_unref)

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
