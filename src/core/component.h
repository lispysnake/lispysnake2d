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

/**
 * Ls2DComponent is a basic block of behaviour for any given entity,
 * and must be added to an Ls2DEntity.
 */
struct Ls2DComponent {
        Ls2DObject object;
        Ls2DEntity *parent_entity;
        int comp_id; /**<Public component ID */

        /* Draw callback that all components should implemented */
        void (*draw)(struct Ls2DComponent *, Ls2DTextureCache *, Ls2DFrameInfo *);

        /* Update callback that all components should implement */
        void (*update)(struct Ls2DComponent *, Ls2DTextureCache *, Ls2DFrameInfo *);

        /* The component needs to be constructed */
        void (*init)(struct Ls2DComponent *, Ls2DTextureCache *, Ls2DFrameInfo *);
};

void ls2d_component_init(Ls2DComponent *self, Ls2DTextureCache *, Ls2DFrameInfo *frame);

/**
 * Inform the component it needs to draw now.
 */
void ls2d_component_draw(Ls2DComponent *self, Ls2DTextureCache *, Ls2DFrameInfo *info);

/**
 * Inform the component it needs to update now.
 */
void ls2d_component_update(Ls2DComponent *self, Ls2DTextureCache *, Ls2DFrameInfo *info);

/**
 * Return component ID
 */
int ls2d_component_get_id(Ls2DComponent *self);

/**
 * Set the parent entity for this component
 * @TODO: Make this private API.
 */
void ls2d_component_set_parent_entity(Ls2DComponent *self, Ls2DEntity *entity);

/**
 * Grab the parent entity
 */
Ls2DEntity *ls2d_component_get_parent_entity(Ls2DComponent *self);

Ls2DComponent *ls2d_component_unref(Ls2DComponent *self);

DEF_AUTOFREE(Ls2DComponent, ls2d_component_unref)

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
