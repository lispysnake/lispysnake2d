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

#include <libxml/xmlreader.h>

#include "ls2d.h"

/**
 * Opaque Ls2DSpriteSheet implementation
 */
struct Ls2DSpriteSheet {
        Ls2DObject object; /*< Parent */

        LsHashmap *textures; /*< Cache of textures in a hashmap */
        Ls2DTextureCache *cache;
};

/**
 * Private API headers for the Ls2DSpriteSheet implementation
 */

/**
 * XML instance parser
 */
typedef struct Ls2DSpriteSheetXML {
        bool in_atlas;
        bool in_subtexture;
        struct {
                Ls2DTextureHandle handle;
        } texture;
        struct {
                int width;
                int height;
                int x;
                int y;
                char *filename;
        } subtexture;
} Ls2DSpriteSheetXML;

/**
 * Attempt to load XML file.
 */
bool ls2d_sprite_sheet_parse_xml(Ls2DSpriteSheet *self, const char *filename);

DEF_AUTOFREE(xmlTextReader, xmlFreeTextReader)
DEF_AUTOFREE(xmlChar, xmlFree)

static inline void ls2d_sprite_sheet_get_int_attr(xmlTextReader *reader, int *storage,
                                                  const char *id)
{
        autofree(xmlChar) *attr = NULL;

        /* TODO: Proper error checking of atoi, etc. */
        attr = xmlTextReaderGetAttribute(reader, BAD_CAST id);
        if (!attr) {
                *storage = 0;
                return;
        }
        *storage = atoi((const char *)attr);
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
