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

#define _GNU_SOURCE

#include <SDL.h>
#include <stdlib.h>
#include <string.h>

#include "libls.h"
#include "texture-cache.h"

#define DEFAULT_CACHE_SIZE 200

static void ls2d_texture_cache_destroy(Ls2DTextureCache *self);

/**
 * Opaque Ls2DTextureCache implementation
 */
struct Ls2DTextureCache {
        Ls2DObject object; /*< Parent */

        LsArray *cache; /*< Cache of textures in an array */
};

typedef struct Ls2DTextureNode {
        SDL_Texture *texture; /**< The real SDL_Texture */
        SDL_Rect area;        /**< Displayable area for the texture. */
        bool subregion;       /**< Whether this node is a subregion. */
        char *filename;       /**<The filename we come from */
} Ls2DTextureNode;

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable texture_cache_vtable = {
        .destroy = (ls2d_object_vfunc_destroy)ls2d_texture_cache_destroy,
        .obj_name = "Ls2DTextureCache",
};

Ls2DTextureCache *ls2d_texture_cache_new()
{
        Ls2DTextureCache *self = NULL;

        self = calloc(1, sizeof(struct Ls2DTextureCache));
        if (ls_unlikely(!self)) {
                return NULL;
        }
        self->cache = ls_array_new_size(sizeof(void *), DEFAULT_CACHE_SIZE);
        if (ls_unlikely(!self->cache)) {
                free(self);
                return NULL;
        }

        return ls2d_object_init((Ls2DObject *)self, &texture_cache_vtable);
}

Ls2DTextureCache *ls2d_texture_cache_unref(Ls2DTextureCache *self)
{
        return ls2d_object_unref(self);
}

/**
 * Clear out the allocated texture.
 */
static void clear_texture(void *v)
{
        Ls2DTextureNode *node = v;

        if (node->subregion) {
                return;
        }

        if (node->texture) {
                SDL_DestroyTexture(node->texture);
        }
        if (node->filename) {
                free(node->filename);
        }
}

static void ls2d_texture_cache_destroy(Ls2DTextureCache *self)
{
        if (self->cache) {
                ls_array_free(self->cache, clear_texture);
        }
}

Ls2DTextureHandle ls2d_texture_cache_load_file(Ls2DTextureCache *self, const char *filename)
{
        ls_array_add(self->cache, NULL);
        uint32_t index = (uint32_t)self->cache->len - 1;
        Ls2DTextureNode *node = self->cache->data[index];

        node->texture = NULL;
        node->subregion = false;
        node->filename = strdup(filename);

        return (Ls2DTextureHandle)index;
}

bool ls2d_texture_cache_draw(Ls2DTextureCache *self, Ls2DFrameInfo *frame, SDL_Rect where,
                             Ls2DTextureHandle handle)
{
        if (ls_unlikely(!self) || ls_unlikely(handle > self->cache->len)) {
                return false;
        }

        return true;
}

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
