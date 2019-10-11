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

#include "spritesheet-private.h"

static void ls2d_sprite_sheet_init(Ls2DSpriteSheet *self);
static void ls2d_sprite_sheet_destroy(Ls2DSpriteSheet *self);

/**
 * We don't yet do anything fancy.
 */
Ls2DObjectTable sprite_sheet_vtable = {
        .init = (ls2d_object_vfunc_init)ls2d_sprite_sheet_init,
        .destroy = (ls2d_object_vfunc_destroy)ls2d_sprite_sheet_destroy,
        .obj_name = "Ls2DSpriteSheet",
};

static void ls2d_sprite_sheet_init(Ls2DSpriteSheet *self)
{
        self->textures =
            ls_hashmap_new_full(ls_hashmap_string_hash, ls_hashmap_string_equal, free, NULL);
}

Ls2DSpriteSheet *ls2d_sprite_sheet_new(Ls2DTextureCache *cache, const char *xml_file)
{
        Ls2DSpriteSheet *self = LS2D_NEW(Ls2DSpriteSheet, sprite_sheet_vtable);

        if (ls_unlikely(!self)) {
                return NULL;
        }

        self->cache = cache;
        if (!ls2d_sprite_sheet_parse_xml(self, xml_file)) {
                ls2d_object_unref((Ls2DObject *)self);
                return NULL;
        }

        return self;
}

Ls2DSpriteSheet *ls2d_sprite_sheet_unref(Ls2DSpriteSheet *self)
{
        return ls2d_object_unref(self);
}

static void ls2d_sprite_sheet_destroy(Ls2DSpriteSheet *self)
{
        ls_hashmap_free(self->textures);
        free(self);
}

Ls2DTextureHandle ls2d_sprite_sheet_lookup(Ls2DSpriteSheet *self, char *key)
{
        if (ls_unlikely(!self)) {
                return 0;
        }
        return (Ls2DTextureHandle)LS_PTR_TO_INT(ls_hashmap_get(self->textures, key));
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
