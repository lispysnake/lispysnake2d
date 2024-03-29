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
 * Ls2DSpriteComponent adds drawable behaviours to an Ls2DEntity.
 */
typedef struct Ls2DSpriteComponent Ls2DSpriteComponent;

/**
 * Construct a new Sprite object.
 */
Ls2DComponent *ls2d_sprite_component_new(void);

/**
 * Unref an allocated Sprite. This will also deference any
 * attached resources.
 */
Ls2DSpriteComponent *ls2d_sprite_component_unref(Ls2DSpriteComponent *self);

void ls2d_sprite_component_set_texture(Ls2DSpriteComponent *self, Ls2DTextureHandle handle);

void ls2d_sprite_component_set_flip(Ls2DSpriteComponent *self, SDL_RendererFlip flip);

void ls2d_sprite_component_set_rotation(Ls2DSpriteComponent *self, double rotation);

DEF_AUTOFREE(Ls2DSpriteComponent, ls2d_sprite_component_unref)

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
