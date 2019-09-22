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

/**
 * Ls2DSprite is used to provide a renderable *thing* within the
 * world. In short, it is the thing that is drawn.
 */
typedef struct Ls2DSprite Ls2DSprite;

/**
 * Construct a new Sprite object.
 */
Ls2DSprite *ls2d_sprite_new(void);

/**
 * Unref an allocated Sprite. This will also deference any
 * attached resources.
 */
Ls2DSprite *ls2d_sprite_unref(Ls2DSprite *self);

DEF_AUTOFREE(Ls2DSprite, ls2d_sprite_unref)

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
