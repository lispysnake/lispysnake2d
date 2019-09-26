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

static void ls2d_input_manager_destroy(Ls2DInputManager *self);

/**
 * Opaque Ls2DInputManager implementation
 */
struct Ls2DInputManager {
        Ls2DObject object; /*< Parent */
        LsHashmap *key_callbacks;

        struct {
                ls2d_mouse_button_callback cb;
                void *userdata;
        } mouse_button;

        struct {
                ls2d_mouse_motion_callback cb;
                void *userdata;
        } mouse_motion;
};

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable input_manager_vtable = {
        .destroy = (ls2d_object_vfunc_destroy)ls2d_input_manager_destroy,
        .obj_name = "Ls2DInputManager",
};

Ls2DInputManager *ls2d_input_manager_new()
{
        Ls2DInputManager *self = NULL;

        self = calloc(1, sizeof(struct Ls2DInputManager));
        if (ls_unlikely(!self)) {
                return NULL;
        }

        self->key_callbacks = ls_hashmap_new(ls_hashmap_simple_hash, ls_hashmap_simple_equal);
        if (ls_unlikely(!self->key_callbacks)) {
                ls2d_input_manager_destroy(self);
                return NULL;
        }

        return ls2d_object_init((Ls2DObject *)self, &input_manager_vtable);
}

Ls2DInputManager *ls2d_input_manager_unref(Ls2DInputManager *self)
{
        return ls2d_object_unref(self);
}

static void ls2d_input_manager_destroy(Ls2DInputManager *self)
{
        if (ls_likely(self->key_callbacks != NULL)) {
                ls_hashmap_free(self->key_callbacks);
        }
}

static inline bool ls2d_input_manager_process_key(Ls2DInputManager *self, SDL_KeyboardEvent *event,
                                                  Ls2DFrameInfo *frame)
{
        return false;
}

static inline bool ls2d_input_manager_process_mouse_button(Ls2DInputManager *self,
                                                           SDL_MouseButtonEvent *event,
                                                           Ls2DFrameInfo *frame)
{
        if (ls_unlikely(!self->mouse_button.cb)) {
                return false;
        }
        return self->mouse_button.cb(event, frame, self->mouse_button.userdata);
}

static inline bool ls2d_input_manager_process_mouse_motion(Ls2DInputManager *self,
                                                           SDL_MouseMotionEvent *event,
                                                           Ls2DFrameInfo *frame)
{
        if (ls_unlikely(!self->mouse_motion.cb)) {
                return false;
        }
        return self->mouse_motion.cb(event, frame, self->mouse_motion.userdata);
}

bool ls2d_input_manager_process(Ls2DInputManager *self, SDL_Event *event, Ls2DFrameInfo *frame)
{
        switch (event->type) {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
                return ls2d_input_manager_process_key(self, &event->key, frame);
        case SDL_MOUSEMOTION:
                return ls2d_input_manager_process_mouse_motion(self, &event->motion, frame);
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
                return ls2d_input_manager_process_mouse_button(self, &event->button, frame);
        default:
                return false;
        }
}

void ls2d_input_manager_set_mouse_button_callback(Ls2DInputManager *self,
                                                  ls2d_mouse_button_callback cb, void *userdata)
{
        if (ls_unlikely(!self)) {
                return;
        }
        if (ls_unlikely(!cb)) {
                self->mouse_button.cb = cb;
                self->mouse_button.userdata = userdata;
        } else {
                self->mouse_button.cb = NULL;
                self->mouse_button.userdata = userdata;
        }
}

void ls2d_input_manager_set_mouse_motion_callback(Ls2DInputManager *self,
                                                  ls2d_mouse_motion_callback cb, void *userdata)
{
        if (ls_unlikely(!self)) {
                return;
        }
        if (ls_unlikely(!cb)) {
                self->mouse_motion.cb = cb;
                self->mouse_motion.userdata = userdata;
        } else {
                self->mouse_motion.cb = NULL;
                self->mouse_motion.userdata = userdata;
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
