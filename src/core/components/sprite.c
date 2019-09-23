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

#include <SDL_image.h>
#include <stdlib.h>

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
        SDL_Texture *texture;
};

static void ls2d_sprite_component_destroy(Ls2DComponent *self);
static void ls2d_sprite_component_draw(Ls2DComponent *self, Ls2DFrameInfo *frame);
static void ls2d_sprite_component_init(Ls2DComponent *self);

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable sprite_vtable = {
        .destroy = (ls2d_object_vfunc_destroy)ls2d_sprite_component_destroy,
        .obj_name = "Ls2DSpriteComponent",
};

DEF_AUTOFREE(SDL_Surface, SDL_FreeSurface)

static SDL_Texture *load_texture(const char *path, SDL_Renderer *ren, SDL_Window *window)
{
        autofree(SDL_Surface) *img_surface = NULL;
        autofree(SDL_Surface) *opt_surface = NULL;
        SDL_Surface *win_surface = NULL;

        img_surface = IMG_Load(path);
        if (!img_surface) {
                fprintf(stderr, "Failed to load %s: %s\n", path, IMG_GetError());
                return NULL;
        }

        /* Mask magenta as transparent */
        SDL_SetColorKey(img_surface, SDL_TRUE, 0xFF00FF);

        /* If the window has no surface, we can't optimize it */
        win_surface = SDL_GetWindowSurface(window);
        if (!win_surface) {
                return SDL_CreateTextureFromSurface(ren, img_surface);
        }

        /* Try to optimize it */
        opt_surface = SDL_ConvertSurface(img_surface, win_surface->format, 0);
        if (!opt_surface) {
                fprintf(stderr, "Failed to optimize surface %s: %s\n", path, SDL_GetError());
                return SDL_CreateTextureFromSurface(ren, img_surface);
        }
        return SDL_CreateTextureFromSurface(ren, opt_surface);
}

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

static void ls2d_sprite_component_destroy(Ls2DComponent *component)
{
        Ls2DSpriteComponent *self = (Ls2DSpriteComponent *)component;

        if (self->texture) {
                SDL_DestroyTexture(self->texture);
        }
}

static void ls2d_sprite_component_init(Ls2DComponent *component)
{
        Ls2DSpriteComponent *self = (Ls2DSpriteComponent *)component;

        self->area = (SDL_Rect){ 500, 500, 100, 100 };
}

/**
 * Perform the actual sprite drawing. Long story short, we need to
 * draw to our X, Y coordinates if within clip using our set
 * texture.
 */
void ls2d_sprite_component_draw(Ls2DComponent *component, Ls2DFrameInfo *frame)
{
        Ls2DSpriteComponent *self = (Ls2DSpriteComponent *)component;

        // 	<SubTexture name="spaceShips_005.png" x="344" y="1050" width="136" height="84"/>
        // 	<SubTexture name="spaceShips_005.png" x="440" y="800" width="342" height="301"/>

        // TODO: Move into init!
        if (!self->texture) {
                self->texture =
                    load_texture("demo_data/Spritesheet/spaceShooter2_spritesheet_2X.png",
                                 frame->renderer,
                                 frame->window);
        }
        // TODO: Use a blit approach and single SDL_RenderCopy which will be far more efficient.
        {
                SDL_Rect src = { 440, 800, 342, 301 };
                SDL_Rect dst = { self->area.x, self->area.y, 342, 301 };
                SDL_RenderCopy(frame->renderer, self->texture, &src, &dst);
        }
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
