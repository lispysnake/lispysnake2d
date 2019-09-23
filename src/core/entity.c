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

#include "entity.h"

/**
 * Inform the entity that all components need to draw now
 */
void ls2d_entity_draw(Ls2DEntity *self, Ls2DFrameInfo *frame)
{
        for (unsigned int i = 0; i < self->n_static_components; i++) {
                Ls2DComponent *comp = &self->static_components[i];
                ls2d_component_draw(comp, frame);
        }
}

/**
 * Inform the entity that all components need to update now.
 */
void ls2d_entity_update(Ls2DEntity *self, Ls2DFrameInfo *frame)
{
        for (unsigned int i = 0; i < self->n_static_components; i++) {
                Ls2DComponent *comp = &self->static_components[i];
                ls2d_component_update(comp, frame);
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
