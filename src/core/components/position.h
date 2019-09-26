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
 * Ls2DPositionComponent adds drawable behaviours to an Ls2DEntity.
 */
typedef struct Ls2DPositionComponent Ls2DPositionComponent;

/**
 * Construct a new Sprite object.
 */
Ls2DComponent *ls2d_position_component_new(void);

/**
 * Unref an allocated Sprite. This will also deference any
 * attached resources.
 */
Ls2DPositionComponent *ls2d_position_component_unref(Ls2DPositionComponent *self);

/**
 * Set X and Y coordinates
 */
void ls2d_position_component_set_xy(Ls2DPositionComponent *self, SDL_Point pos);

/**
 * Set the Z position
 */
void ls2d_position_component_set_z(Ls2DPositionComponent *self, int z);

/**
 * Get the Z position
 */
bool ls2d_position_component_get_z(Ls2DPositionComponent *self, int *z);

/**
 * Get the X and Y coordinates into pos
 */
bool ls2d_position_component_get_xy(Ls2DPositionComponent *self, SDL_Point *pos);

DEF_AUTOFREE(Ls2DPositionComponent, ls2d_position_component_unref)

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
