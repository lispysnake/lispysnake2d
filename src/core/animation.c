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

static void ls2d_animation_destroy(Ls2DAnimation *self);

struct Ls2DAnimation {
        Ls2DObject parent;

        /* Dynamic storage of frames */
        LsArray *frames;
        uint16_t cur_frame;
        Ls2DTextureHandle handle;
        bool looping;
        bool playing;
};

/**
 * Animation frame is used internally so we can quickly cycle
 * animation frames and retain a linear cache.
 */
typedef struct Ls2DAnimationFrame {
        uint32_t duration;
        Ls2DTextureHandle handle;
} Ls2DAnimationFrame;

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable animation_vtable = {
        .destroy = (ls2d_object_vfunc_destroy)ls2d_animation_destroy,
        .obj_name = "Ls2DAnimation",
};

Ls2DAnimation *ls2d_animation_new()
{
        Ls2DAnimation *self = NULL;

        self = calloc(1, sizeof(struct Ls2DAnimation));
        if (ls_unlikely(!self)) {
                return NULL;
        }
        self->cur_frame = 0;
        self->looping = true;
        self->playing = true;
        self->frames = ls_array_new_size(sizeof(struct Ls2DAnimationFrame), 3);
        if (ls_unlikely(!self->frames)) {
                free(self);
                return NULL;
        }

        return ls2d_object_init((Ls2DObject *)self, &animation_vtable);
}

Ls2DAnimation *ls2d_animation_unref(Ls2DAnimation *self)
{
        return ls2d_object_unref(self);
}

static void ls2d_animation_destroy(Ls2DAnimation *self)
{
        ls_array_free(self->frames, NULL);
}

__attribute__((always_inline)) static inline Ls2DAnimationFrame *lookup_frame(void *cache,
                                                                              uint16_t index)
{
        Ls2DAnimationFrame *root = cache;
        return &(root[index]);
}

bool ls2d_animation_add_frame(Ls2DAnimation *self, Ls2DTextureHandle handle, uint32_t duration)
{
        Ls2DAnimationFrame *frame = NULL;
        uint16_t index = 0;

        if (ls_unlikely(!self)) {
                return false;
        }
        if (ls_unlikely(!ls_array_add(self->frames, NULL))) {
                return false;
        }

        index = (uint16_t)(self->frames->len - 1);
        frame = lookup_frame(self->frames->data, index);
        if (ls_unlikely(!frame)) {
                return false;
        }
        frame->handle = handle;
        frame->duration = duration;
        return true;
}

void ls2d_animation_update(Ls2DAnimation *self, Ls2DFrameInfo *frame)
{
        Ls2DAnimationFrame *cur_frame = NULL;

        if (ls_unlikely(!self)) {
                return;
        }

        if (ls_unlikely(!self->playing)) {
                return;
        }

        if (ls_unlikely(self->frames->len < 1)) {
                return;
        }

        cur_frame = lookup_frame(self->frames, self->cur_frame);
        self->handle = cur_frame->handle;

        /* If we don't need to update... don't */
        if (frame->ticks < cur_frame->duration) {
                return;
        }

        if (self->cur_frame > self->frames->len - 1) {
                if (self->looping) {
                        ls2d_animation_stop(self);
                        return;
                }
                self->cur_frame = 0;
        }

        cur_frame = lookup_frame(self->frames, self->cur_frame);
        self->handle = cur_frame->handle;
}

void ls2d_animation_stop(Ls2DAnimation *self)
{
        if (ls_unlikely(!self)) {
                return;
        }
        self->playing = false;
}

void ls2d_animation_set_looping(Ls2DAnimation *self, bool looping)
{
        if (ls_unlikely(!self)) {
                return;
        }
        self->looping = looping;
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
