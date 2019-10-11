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

#include <fcntl.h>
#include <libxml/xmlreader.h>
#include <unistd.h>

#include "ls2d.h"
#include "tilesheet-private.h"

static void ls2d_tile_sheet_walk_xml(Ls2DTileSheet *self, Ls2DTileSheetXML *parser,
                                     xmlTextReader *reader);

/**
 * Attempt to load XML file.
 */
bool ls2d_tile_sheet_parse_xml(Ls2DTileSheet *self, const char *filename)
{
        int fd = 0;
        autofree(xmlTextReader) *reader = NULL;
        bool ret = false;
        int r = 0;
        Ls2DTileSheetXML parser = { 0 };

        fd = open(filename, O_RDONLY);
        if (fd < 0) {
                goto fail;
        }

        /* Not portable. Improve for Windows. */
        (void)posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);

        reader = xmlReaderForFd(fd, filename, NULL, 0);
        if (!reader) {
                goto fail;
        }

        while ((r = xmlTextReaderRead(reader)) > 0) {
                ls2d_tile_sheet_walk_xml(self, &parser, reader);
        }
        ret = true;

fail:
        if (fd >= 0) {
                close(fd);
        }

        return ret;
}

/**
 * Walk each node and then process it.
 */
static void ls2d_tile_sheet_walk_xml(Ls2DTileSheet *self, Ls2DTileSheetXML *parser,
                                     xmlTextReader *reader)
{
        const xmlChar *name = NULL;

        name = xmlTextReaderConstName(reader);
        if (!name) {
                return;
        }

        /* Are we in the texture atlas..? */
        if (xmlStrEqual(name, BAD_CAST "TextureAtlas")) {
                autofree(xmlChar) *image_path = NULL;

                parser->in_atlas = !parser->in_atlas;
                if (!parser->in_atlas) {
                        return;
                }
                /* We grabbed the texture filename */
                image_path = xmlTextReaderGetAttribute(reader, BAD_CAST "imagePath");
                parser->texture.handle =
                    ls2d_texture_cache_load_file(self->cache, (char *)image_path);
                /* TODO: Check handle validity??? */
        }

        if (parser->in_atlas && xmlStrEqual(name, BAD_CAST "SubTexture")) {
                autofree(xmlChar) *filepath = NULL;
                Ls2DTextureHandle subhandle = 0;

                parser->in_subtexture = !parser->in_subtexture;
                ls2d_tile_sheet_get_int_attr(reader, &parser->subtexture.x, "x");
                ls2d_tile_sheet_get_int_attr(reader, &parser->subtexture.y, "y");
                ls2d_tile_sheet_get_int_attr(reader, &parser->subtexture.width, "width");
                ls2d_tile_sheet_get_int_attr(reader, &parser->subtexture.height, "height");

                filepath = xmlTextReaderGetAttribute(reader, BAD_CAST "name");
                if (!filepath) {
                        fprintf(stderr, "Missing filepath for SubTexture. Dropping.\n");
                        return;
                }
                subhandle = ls2d_texture_cache_subregion(self->cache,
                                                         parser->texture.handle,
                                                         (SDL_Rect){
                                                             .x = parser->subtexture.x,
                                                             .y = parser->subtexture.y,
                                                             .w = parser->subtexture.width,
                                                             .h = parser->subtexture.height,
                                                         });
                ls2d_tile_sheet_put_handle(self, strdup((const char *)filepath), subhandle);
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
