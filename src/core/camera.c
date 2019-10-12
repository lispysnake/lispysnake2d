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

#include <assert.h>

#include "ls2d.h"

struct Ls2DCamera {
        Ls2DObject parent;
        SDL_Rect world_bounds; /**< The current world size. */
        SDL_Rect view;         /**< The current view size */
        SDL_Point look_at;     /**<Where we're aimed at. */
        Ls2DScene *scene;
};

Ls2DObjectTable camera_vtable = {
        .obj_name = "Ls2DCamera",
};

Ls2DCamera *ls2d_camera_new(Ls2DScene *scene)
{
        Ls2DCamera *self = LS2D_NEW(Ls2DCamera, camera_vtable);

        if (ls_unlikely(!self)) {
                return NULL;
        }

        self->scene = scene;
        return self;
}

Ls2DCamera *ls2d_camera_unref(Ls2DCamera *self)
{
        return ls2d_object_unref(self);
}

void ls2d_camera_set_world_bounds(Ls2DCamera *self, SDL_Rect world_bounds)
{
        if (ls_unlikely(!self)) {
                return;
        }
        assert(world_bounds.w > 0);
        assert(world_bounds.h > 0);
        self->world_bounds = world_bounds;
}

void ls2d_camera_set_xy(Ls2DCamera *self, SDL_Point look_at)
{
        if (ls_unlikely(!self)) {
                return;
        }
        self->look_at = look_at;
        if (self->look_at.x < 0) {
                self->look_at.x = 0;
        }
        if (self->look_at.y < 0) {
                self->look_at.y = 0;
        }
        /*
        if (self->look_at.x + self->view.w > self->world_bounds.w) {
                self->look_at.x = self->world_bounds.w - self->view.w;
        }*/
        if (self->look_at.y + self->view.h > self->world_bounds.h) {
                self->look_at.y = self->world_bounds.h - self->view.h;
        }
        if (self->look_at.x + self->view.w > self->world_bounds.w) {
                self->look_at.x = self->world_bounds.w - self->view.w;
        }
        /* TODO: Update camera properly. */
}

static bool ls2d_camera_get_entity_position(Ls2DEntity *entity, int *pos_x, int *pos_y)
{
        *pos_x = 0;
        *pos_y = 0;

        Ls2DComponent *position = NULL;
        SDL_Point real_pos = { 0 };
        position = ls2d_entity_get_component(entity, LS2D_COMP_ID_POSITION);
        if (!ls2d_position_component_get_xy((Ls2DPositionComponent *)position, &real_pos)) {
                return false;
        }
        *pos_x = real_pos.x;
        *pos_y = real_pos.y;
        return true;
}

bool ls2d_camera_convert_entity_position(Ls2DCamera *self, Ls2DEntity *entity, int *pos_x,
                                         int *pos_y)
{
        int r_pos_x = 0, r_pos_y = 0;

        if (ls_unlikely(!self) || ls_unlikely(!entity)) {
                return false;
        }

        if (!ls2d_camera_get_entity_position(entity, &r_pos_x, &r_pos_y)) {
                return false;
        }

        *pos_x = r_pos_x - self->look_at.x;
        *pos_y = r_pos_y - self->look_at.y;

        return true;
}

bool ls2d_camera_entity_in_bounds(Ls2DCamera *self, Ls2DEntity *entity)
{
        int pos_x = 0, pos_y = 0;

        if (ls_unlikely(!self) || ls_unlikely(!entity)) {
                return false;
        }

        if (!ls2d_camera_get_entity_position(entity, &pos_x, &pos_y)) {
                return true;
        }

        if (pos_x > self->world_bounds.w || pos_x < self->world_bounds.x) {
                return false;
        }
        if (pos_y > self->world_bounds.h || pos_y < self->world_bounds.y) {
                return false;
        }

        return true;
}

void ls2d_camera_update(Ls2DCamera *self, Ls2DFrameInfo *frame)
{
        if (ls_unlikely(!self)) {
                return;
        }
        /* If we have a logical size, use it. Otherwise, real size. */
        SDL_RenderGetLogicalSize(frame->renderer, &self->view.w, &self->view.h);
        if (self->view.w == 0) {
                SDL_GetRendererOutputSize(frame->renderer, &self->view.w, &self->view.h);
        }

        /* TODO: Update camera to point at correct place. */
}

bool ls2d_camera_get_view(Ls2DCamera *self, SDL_Rect *view)
{
        if (ls_unlikely(!self) || ls_unlikely(!view)) {
                return false;
        }
        view->x = self->look_at.x;
        view->y = self->look_at.y;
        view->w = self->view.w;
        view->h = self->view.h;
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
