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

#include "engine.h"
#include "libls.h"
#include "object.h"

/**
 * Ls2DTilesheet is used to provide efficient rendering of a Tilesheet,
 * that is, a large image composed of multiple renderable images. This
 * helps to avoid the so-called power-of-2 problem, and avoid expensive
 * texture switching on the GPU by rendering multiple different images
 * all from one image.
 */
typedef struct Ls2DTilesheet Ls2DTilesheet;

/**
 * Construct a new Tilesheet object.
 */
Ls2DTilesheet *ls2d_tilesheet_new(Ls2DEngine *engine);

/**
 * Unref an allocated Scene. This will also deference any
 * attached resources.
 */
Ls2DTilesheet *ls2d_tilesheet_unref(Ls2DTilesheet *self);

/**
 * Draw the named image at the given X and Y coordinates.
 */
void ls2d_tilesheet_draw(Ls2DTilesheet *self, const char *image_name, int x, int y);

DEF_AUTOFREE(Ls2DTilesheet, ls2d_tilesheet_unref)

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
