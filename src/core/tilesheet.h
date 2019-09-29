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
 * Construct a new Ls2DTileSheet Object
 */
Ls2DTileSheet *ls2d_tile_sheet_new_from_xml(Ls2DTextureCache *cache, const char *xml_path);

/**
 * Return a texture handle corresponding to the given ID
 * This will map to the internal cache's texture handle.
 */
Ls2DTextureHandle ls2d_tile_sheet_lookup(Ls2DTileSheet *self, const char *id);

/**
 * Unref an allocated Ls2DTileSheet
 */
Ls2DTileSheet *ls2d_tile_sheet_unref(Ls2DTileSheet *self);

DEF_AUTOFREE(Ls2DTileSheet, ls2d_tile_sheet_unref)

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
