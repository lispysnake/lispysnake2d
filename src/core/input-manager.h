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

typedef bool (*ls2d_key_callback)(SDL_KeyboardEvent *, Ls2DFrameInfo *, void *);
typedef bool (*ls2d_mouse_button_callback)(SDL_MouseButtonEvent *, Ls2DFrameInfo *, void *);
typedef bool (*ls2d_mouse_motion_callback)(SDL_MouseMotionEvent *, Ls2DFrameInfo *, void *);

/**
 * Construct a new InputManager
 */
Ls2DInputManager *ls2d_input_manager_new(void);

/**
 * Unref a previously allocated InputManager
 */
Ls2DInputManager *ls2d_input_manager_unref(Ls2DInputManager *self);

/**
 * Ensure batching of events occurs
 */
bool ls2d_input_manager_process(Ls2DInputManager *self, SDL_Event *event, Ls2DFrameInfo *frame);

void ls2d_input_manager_set_mouse_button_callback(Ls2DInputManager *self,
                                                  ls2d_mouse_button_callback cb, void *userdata);
void ls2d_input_manager_set_mouse_motion_callback(Ls2DInputManager *self,
                                                  ls2d_mouse_motion_callback cb, void *userdata);

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
