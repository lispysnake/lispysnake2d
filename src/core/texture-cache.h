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

struct Ls2DTextureNode {
        SDL_Texture *texture; /**< The real SDL_Texture */
        SDL_Rect area;        /**< Displayable area for the texture. */
        char *filename;       /**<The filename we come from */
        bool subregion;       /**< Whether this node is a subregion. */
};

/**
 * Construct a new Ls2DTextureCache Object
 */
Ls2DTextureCache *ls2d_texture_cache_new(void);

/**
 * Unref an allocated Ls2DTextureCache
 */
Ls2DTextureCache *ls2d_texture_cache_unref(Ls2DTextureCache *self);

/**
 * Allocate a texture handle by loading from the given file.
 * @note: The texture should ONLY be loaded when an SDL window is available!
 */
Ls2DTextureHandle ls2d_texture_cache_load_file(Ls2DTextureCache *self, const char *filename);

/**
 * Lookup a texture for rendering
 */
const Ls2DTextureNode *ls2d_texture_cache_lookup(Ls2DTextureCache *self, Ls2DFrameInfo *frame,
                                                 Ls2DTextureHandle handle);

DEF_AUTOFREE(Ls2DTextureCache, ls2d_texture_cache_unref)

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
