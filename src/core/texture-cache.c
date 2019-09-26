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
#include <SDL_image.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ls2d.h"

#define DEFAULT_CACHE_SIZE 200

DEF_AUTOFREE(SDL_Surface, SDL_FreeSurface)

static void ls2d_texture_cache_destroy(Ls2DTextureCache *self);

/**
 * Opaque Ls2DTextureCache implementation
 */
struct Ls2DTextureCache {
        Ls2DObject object; /*< Parent */

        LsArray *cache; /*< Cache of textures in an array */
};

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
        self->cache = ls_array_new_size(sizeof(struct Ls2DTextureNode), DEFAULT_CACHE_SIZE);
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

static SDL_Texture *load_texture_internal(Ls2DTextureNode *node, Ls2DFrameInfo *frame)
{
        autofree(SDL_Surface) *img_surface = NULL;
        autofree(SDL_Surface) *opt_surface = NULL;
        SDL_Surface *win_surface = NULL;

        img_surface = IMG_Load(node->filename);
        if (!img_surface) {
                fprintf(stderr, "Failed to load %s: %s\n", node->filename, IMG_GetError());
                return NULL;
        }

        /* Mask magenta as transparent */
        // SDL_SetColorKey(img_surface, SDL_TRUE, 0xFF00FF);

        /* If the window has no surface, we can't optimize it */
        win_surface = SDL_GetWindowSurface(frame->window);
        if (!win_surface) {
                return SDL_CreateTextureFromSurface(frame->renderer, img_surface);
        }

        /* Try to optimize it */
        opt_surface = SDL_ConvertSurface(img_surface, win_surface->format, 0);
        if (!opt_surface) {
                fprintf(stderr,
                        "Failed to optimize surface %s: %s\n",
                        node->filename,
                        SDL_GetError());
                return SDL_CreateTextureFromSurface(frame->renderer, img_surface);
        }
        return SDL_CreateTextureFromSurface(frame->renderer, opt_surface);
}

static SDL_Texture *load_texture(Ls2DTextureNode *node, Ls2DFrameInfo *frame)
{
        SDL_Texture *texture = NULL;

        texture = load_texture_internal(node, frame);
        if (ls_unlikely(!texture)) {
                return NULL;
        }

        /* Find out real width and height
         * TODO: Don't bother for subregions.
         */
        SDL_QueryTexture(texture, NULL, NULL, &node->area.w, &node->area.h);
        return texture;
}

/**
 * Clear out the allocated texture.
 */
static void clear_texture(Ls2DTextureNode *node)
{
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
        for (uint16_t i = 0; i < self->cache->len; i++) {
                clear_texture(&((Ls2DTextureNode *)self->cache->data)[i]);
        }
        ls_array_free(self->cache, NULL);
}

Ls2DTextureHandle ls2d_texture_cache_load_file(Ls2DTextureCache *self, const char *filename)
{
        struct Ls2DTextureNode *node = NULL;
        uint32_t index = 0;

        /* Preallocate cached texture */
        ls_array_add(self->cache, NULL);
        index = (uint32_t)self->cache->len - 1;
        node = &((Ls2DTextureNode *)self->cache->data)[index];

        /* Sort out the cache */
        node->filename = strdup(filename);
        node->subregion = false;
        node->texture = NULL;

        return (Ls2DTextureHandle)index;
}

Ls2DTextureHandle ls2d_texture_cache_subregion(Ls2DTextureCache *self, Ls2DTextureHandle parent,
                                               SDL_Rect subregion)
{
        struct Ls2DTextureNode *parent_node = NULL;
        struct Ls2DTextureNode *node = NULL;
        uint32_t index = 0;

        if (ls_unlikely(!self)) {
                return 0;
        }
        if (ls_unlikely(parent > self->cache->len)) {
                return 0;
        }

        parent_node = &((Ls2DTextureNode *)self->cache->data)[parent];
        assert(parent_node->subregion != true);

        /* Preallocate cached texture */
        ls_array_add(self->cache, NULL);
        index = (uint32_t)self->cache->len - 1;
        node = &((Ls2DTextureNode *)self->cache->data)[index];

        /* Sort out the cache */
        node->filename = NULL;
        node->subregion = true;
        node->area = subregion;
        node->texture = NULL;
        node->parent = parent_node;

        return (Ls2DTextureHandle)index;
}

const Ls2DTextureNode *ls2d_texture_cache_lookup(Ls2DTextureCache *self, Ls2DFrameInfo *frame,
                                                 Ls2DTextureHandle handle)
{
        Ls2DTextureNode *node = NULL;

        if (ls_unlikely(!self)) {
                return NULL;
        }
        if (ls_unlikely(handle > self->cache->len)) {
                return NULL;
        }

        node = &((Ls2DTextureNode *)self->cache->data)[handle];
        if (!node->texture) {
                if (node->parent && !node->parent->texture) {
                        node->parent->texture = load_texture(node->parent, frame);
                        node->texture = node->parent->texture;
                } else {
                        node->texture = load_texture(node, frame);
                }
        }

        return (const Ls2DTextureNode *)node;
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
