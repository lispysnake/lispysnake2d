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

#include <string.h>

#include "ls2d.h"

static void ls2d_animation_component_destroy(Ls2DAnimationComponent *self);
static void ls2d_animation_component_update(Ls2DComponent *component, Ls2DTextureCache *cache,
                                            Ls2DFrameInfo *frame);

/**
 * Opaque Ls2DAnimationComponent implementation
 */
struct Ls2DAnimationComponent {
        Ls2DComponent parent; /*< Parent */
        LsHashmap *anims;
        Ls2DAnimation *cur_anim;
};

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable animation_component_vtable = {
        .obj_name = "Ls2DAnimationComponent",
        .destroy = (ls2d_object_vfunc_destroy)ls2d_animation_component_destroy,
};

Ls2DComponent *ls2d_animation_component_new()
{
        Ls2DAnimationComponent *self = NULL;

        self = calloc(1, sizeof(struct Ls2DAnimationComponent));
        if (ls_unlikely(!self)) {
                return NULL;
        }
        self->anims = ls_hashmap_new_full(ls_hashmap_string_hash,
                                          ls_hashmap_string_equal,
                                          free,
                                          (ls_hashmap_free_func)ls2d_object_unref);
        if (ls_unlikely(!self->anims)) {
                ls2d_animation_component_destroy(self);
                free(self);
                return NULL;
        }

        self = ls2d_object_init((Ls2DObject *)self, &animation_component_vtable);
        self->parent.comp_id = LS2D_COMP_ID_ANIMATION;
        self->parent.update = ls2d_animation_component_update;

        return (Ls2DComponent *)self;
}

Ls2DAnimationComponent *ls2d_animation_component_unref(Ls2DAnimationComponent *self)
{
        return ls2d_object_unref(self);
}

bool ls2d_animation_component_add_animation(Ls2DAnimationComponent *self, const char *id,
                                            Ls2DAnimation *animation)
{
        char *id_c = NULL;

        if (ls_unlikely(!self) || ls_unlikely(!animation)) {
                return false;
        }

        id_c = strdup(id);
        if (!id_c) {
                return false;
        }

        if (ls_unlikely(!ls_hashmap_put(self->anims, id_c, ls2d_object_ref(animation)))) {
                ls2d_object_unref(animation);
                return false;
        }

        if (ls_unlikely(!self->cur_anim)) {
                self->cur_anim = animation;
                ls2d_animation_reset(self->cur_anim);
        }

        return true;
}

bool ls2d_animation_component_set_animation(Ls2DAnimationComponent *self, const char *id)
{
        Ls2DAnimation *anim = NULL;

        if (ls_unlikely(!self)) {
                return false;
        }
        anim = ls_hashmap_get(self->anims, (char *)id);
        if (ls_unlikely(!anim)) {
                return false;
        }
        self->cur_anim = anim;
        ls2d_animation_reset(self->cur_anim);
        return true;
}

static void ls2d_animation_component_destroy(Ls2DAnimationComponent *self)
{
        if (ls_likely(self->anims != NULL)) {
                ls_hashmap_free(self->anims);
        }
}

static void ls2d_animation_component_update(Ls2DComponent *component,
                                            __ls_unused__ Ls2DTextureCache *cache,
                                            Ls2DFrameInfo *frame)
{
        Ls2DAnimationComponent *self = NULL;

        if (ls_unlikely(!component)) {
                return;
        }

        self = (Ls2DAnimationComponent *)component;
        if (ls_unlikely(!self->cur_anim)) {
                return;
        }
        ls2d_animation_update(self->cur_anim, frame);
}

Ls2DTextureHandle ls2d_animation_component_get_texture(Ls2DAnimationComponent *self)
{
        if (ls_unlikely(!self)) {
                return 0;
        }
        return ls2d_animation_get_texture(self->cur_anim);
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
