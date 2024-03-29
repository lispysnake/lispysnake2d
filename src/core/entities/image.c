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

#include "ls2d.h"
#include "tilemap-private.h"

static void ls2d_image_destroy(Ls2DImage *self);
static void ls2d_image_init(Ls2DImage *self);
static void ls2d_image_draw(Ls2DEntity *entity, Ls2DTextureCache *cache, Ls2DFrameInfo *frame);
static void ls2d_image_update(Ls2DEntity *entity, Ls2DTextureCache *cache, Ls2DFrameInfo *frame);

struct Ls2DImage {
        Ls2DEntity parent;
        Ls2DTextureHandle handle;
};

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable image_vtable = {
        .obj_name = "Ls2DImage",
        .init = (ls2d_object_vfunc_init)ls2d_image_init,
        .destroy = (ls2d_object_vfunc_destroy)ls2d_image_destroy,
};

Ls2DEntity *ls2d_image_new(Ls2DTextureHandle handle)
{
        Ls2DImage *self = LS2D_NEW(Ls2DImage, image_vtable);
        if (ls_unlikely(!self)) {
                return NULL;
        }
        self->handle = handle;
        return (Ls2DEntity *)self;
}

static void ls2d_image_init(Ls2DImage *self)
{
        self->parent.draw = ls2d_image_draw;
        self->parent.update = ls2d_image_update;
}

Ls2DImage *ls2d_image_unref(Ls2DImage *self)
{
        return ls2d_object_unref(self);
}

static void ls2d_image_destroy(Ls2DImage *self)
{
        free(self);
}

static void ls2d_image_draw(Ls2DEntity *entity, Ls2DTextureCache *cache, Ls2DFrameInfo *frame)
{
        Ls2DImage *self = (Ls2DImage *)entity;
        const Ls2DTextureNode *node = NULL;
        SDL_Rect view = { 0 };
        SDL_Rect draw = { 0 };
        SDL_Rect source = { 0 };
        SDL_Rect bounds = { 0 };

        node = ls2d_texture_cache_lookup(cache, frame, self->handle);
        if (ls_unlikely(!node)) {
                return;
        }

        if (!ls2d_camera_get_view(frame->camera, &view)) {
                return;
        }
        if (!ls2d_camera_get_world_bounds(frame->camera, &bounds)) {
                return;
        }

        draw.x = 0;
        draw.y = 0;
        /* Set the X offset */
        source.x = node->area.x;
        source.y = node->area.y;

        /* Clamp renderable width to camera */
        source.w = view.w;
        source.h = view.h;
        draw.w = source.w;
        draw.h = source.h;

        double fraction = (double)view.x / (double)bounds.w;
        double mid_x = node->area.w / 2.0;
        mid_x *= 0.15;
        double new_x = mid_x * fraction;
        source.x = new_x;

        fraction = (double)view.y / (double)bounds.h;
        double mid_y = node->area.h / 2.0;
        mid_y *= 0.15;
        double new_y = mid_y * fraction;

        source.x = new_x;
        source.y = new_y;

        SDL_RenderCopy(frame->renderer, node->texture, &source, &draw);
}

static void ls2d_image_update(Ls2DEntity *entity, Ls2DTextureCache *cache, Ls2DFrameInfo *frame)
{
        /* TODO: Update on camera offsets */
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
