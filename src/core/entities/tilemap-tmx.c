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

#include "tilemap-private.h"

static bool ls2d_tilemap_walk_tmx(Ls2DTileMap *self, Ls2DTileMapTMX *parser, xmlTextReader *reader);

bool ls2d_tilemap_load_tsx(Ls2DTileMap *self, Ls2DTextureCache *cache, const char *filename)
{
        int fd = 0;
        autofree(xmlTextReader) *reader = NULL;
        bool ret = false;
        int r = 0;
        Ls2DTileMapTMX parser = { 0 };
        parser.cache = cache;

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
                if (!ls2d_tilemap_walk_tmx(self, &parser, reader)) {
                        goto fail;
                }
        }
        ret = true;

fail:
        if (fd >= 0) {
                close(fd);
        }

        return ret;
}

static bool ls2d_tilemap_load_csv(Ls2DTileMap *self, Ls2DTileMapTMX *parser, const xmlChar *text)
{
        autofree(char) *dup = strdup((const char *)text);
        int key = 0;
        char *csv = NULL;
        int x = 0, y = 0;

        if (!dup) {
                return false;
        }
        csv = strtok(dup, ",\n");
        while (csv) {
                key = atoi(csv);
                if (!ls2d_tilemap_set_internal(self, parser->layer.id - 1, x, y, key)) {
                        return false;
                }

                ++x;
                if (x >= parser->map.width) {
                        x = 0;
                        y++;
                }
                csv = strtok(NULL, ",\n");
        }
        return true;
}

static bool ls2d_tilemap_walk_tmx(Ls2DTileMap *self, Ls2DTileMapTMX *parser, xmlTextReader *reader)
{
        const xmlChar *name = NULL;
        const xmlChar *value = NULL;

        name = xmlTextReaderConstName(reader);
        if (!name) {
                return true;
        }

        /* Encountered tileset */
        if (parser->in_map && xmlStrEqual(name, BAD_CAST "tileset")) {
                parser->in_tileset = !parser->in_tileset;
                return true;
        }

        /* Encountered layer definition */
        if (parser->in_map && xmlStrEqual(name, BAD_CAST "layer")) {
                parser->in_layer = !parser->in_layer;
                if (!parser->in_layer) {
                        return true;
                }
                ls2d_tilemap_get_int_attr(reader, &parser->layer.id, "id");
                ls2d_tilemap_get_int_attr(reader, &parser->layer.width, "width");
                ls2d_tilemap_get_int_attr(reader, &parser->layer.height, "height");

                fprintf(stderr,
                        "Layer %d, %dx%d\n",
                        parser->layer.id,
                        parser->layer.width,
                        parser->layer.height);

                if (!ls2d_tilemap_add_layer(self, parser->layer.id)) {
                        return false;
                }
                return true;
        }

        /* Encountered data definition */
        if (parser->in_layer && xmlStrEqual(name, BAD_CAST "data")) {
                parser->in_data = !parser->in_data;
                /* TODO: Determine blob type */
                return true;
        }

        /* Encountered map definition */
        if (xmlStrEqual(name, BAD_CAST "map")) {
                parser->in_map = true;
                if (!parser->in_map) {
                        return true;
                }
                /* Handle map attributes */
                ls2d_tilemap_get_int_attr(reader, &parser->map.width, "width");
                ls2d_tilemap_get_int_attr(reader, &parser->map.tile_width, "tilewidth");
                ls2d_tilemap_get_int_attr(reader, &parser->map.height, "height");
                ls2d_tilemap_get_int_attr(reader, &parser->map.tile_height, "tileheight");

                /* Set up basic attributes now */
                self->width = (uint16_t)parser->map.width;
                self->height = (uint16_t)parser->map.height;
                self->size = self->width * self->height;

                /* TODO: Move away from tile_size increments and have tile_height/tile_width */
                self->tile_size = parser->map.tile_width;
                return true;
        }

        if (parser->in_data) {
                value = xmlTextReaderConstValue(reader);
                if (!ls2d_tilemap_load_csv(self, parser, value)) {
                        return false;
                }
        }
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
