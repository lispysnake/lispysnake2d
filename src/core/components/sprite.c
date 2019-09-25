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

#include <stdlib.h>
#include <string.h>

#include "component.h"
#include "libls.h"
#include "object.h"
#include "sprite.h"

/**
 * Opaque Ls2DSpriteComponent implementation
 */
struct Ls2DSpriteComponent {
        Ls2DComponent parent; /*< Parent */
        SDL_Rect area;
        Ls2DTextureHandle handle;
};

static void ls2d_sprite_component_draw(Ls2DComponent *self, Ls2DTextureCache *cache,
                                       Ls2DFrameInfo *frame);
static void ls2d_sprite_component_init(Ls2DComponent *self, Ls2DTextureCache *cache,
                                       Ls2DFrameInfo *frame);
static int offset_x = 100;
static int offset_y = 0;

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable sprite_vtable = {
        .obj_name = "Ls2DSpriteComponent",
};

Ls2DComponent *ls2d_sprite_component_new()
{
        Ls2DSpriteComponent *self = NULL;

        self = calloc(1, sizeof(struct Ls2DSpriteComponent));
        if (ls_unlikely(!self)) {
                return NULL;
        }

        self = ls2d_object_init((Ls2DObject *)self, &sprite_vtable);

        self->parent.init = ls2d_sprite_component_init;
        self->parent.draw = ls2d_sprite_component_draw;

        return (Ls2DComponent *)self;
}

Ls2DSpriteComponent *ls2d_sprite_component_unref(Ls2DSpriteComponent *self)
{
        return ls2d_object_unref(self);
}

Ls2DSpriteComponent *ls2d_sprite_component_set_texture(Ls2DSpriteComponent *self,
                                                       Ls2DTextureHandle handle)
{
        self->handle = handle;
}

static void ls2d_sprite_component_init(Ls2DComponent *component, Ls2DTextureCache *cache,
                                       Ls2DFrameInfo *frame)
{
        Ls2DSpriteComponent *self = (Ls2DSpriteComponent *)component;
        offset_x += 210;
        // offset_y += 0;

        self->area = (SDL_Rect){ 200 + offset_x, 200 + offset_y, 100, 100 };
}

/**
 * Perform the actual sprite drawing. Long story short, we need to
 * draw to our X, Y coordinates if within clip using our set
 * texture.
 */
void ls2d_sprite_component_draw(Ls2DComponent *component, Ls2DTextureCache *cache,
                                Ls2DFrameInfo *frame)
{
        Ls2DSpriteComponent *self = (Ls2DSpriteComponent *)component;
        const Ls2DTextureNode *node = ls2d_texture_cache_lookup(cache, frame, self->handle);
        if (!node) {
                return;
        }

        SDL_Rect dst = { self->area.x, self->area.y, node->area.w, node->area.h };
        if (node->subregion) {
                SDL_RenderCopy(frame->renderer, node->texture, &node->area, &dst);
        } else {
                SDL_RenderCopy(frame->renderer, node->texture, NULL, &dst);
        }

        // 	<SubTexture name="spaceShips_005.png" x="344" y="1050" width="136" height="84"/>
        // 	<SubTexture name="spaceShips_005.png" x="440" y="800" width="342" height="301"/>
        // TODO: Use a blit approach and single SDL_RenderCopy which will be far more efficient.
        // SDL_RenderCopy(frame->renderer, self->texture, &src, &dst);
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
