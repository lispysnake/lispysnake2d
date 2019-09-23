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

#include "component.h"
#include "libls.h"

void ls2d_component_init(Ls2DComponent *self, Ls2DFrameInfo *frame)
{
        if (ls_unlikely(!self) || ls_unlikely(!self->init)) {
                return;
        }
        self->init(self, frame);
}

void ls2d_component_draw(Ls2DComponent *self, Ls2DFrameInfo *info)
{
        if (ls_unlikely(!self) || ls_unlikely(!self->draw)) {
                return;
        }
        self->draw(self, info);
}

void ls2d_component_update(Ls2DComponent *self, Ls2DFrameInfo *info)
{
        if (ls_unlikely(!self) || ls_unlikely(!self->update)) {
                return;
        }
        self->update(self, info);
}

Ls2DComponent *ls2d_component_unref(Ls2DComponent *self)
{
        return ls2d_object_unref(self);
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
