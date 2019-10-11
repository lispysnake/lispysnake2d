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

static void ls2d_tile_sheet_walk_tsx(Ls2DTileSheet *self, Ls2DTileSheetTSX *parser,
                                     xmlTextReader *reader);
static void ls2d_tile_sheet_start_animation(Ls2DTileSheet *self, Ls2DTileSheetTSX *parser);
static void ls2d_tile_sheet_end_animation(Ls2DTileSheet *self, Ls2DTileSheetTSX *parser);
static void ls2d_tile_sheet_add_frame(Ls2DTileSheet *self, Ls2DTileSheetTSX *parser,
                                      xmlTextReader *reader);

/**
 * Attempt to load XML file.
 */
bool ls2d_tile_sheet_parse_tsx(Ls2DTileSheet *self, const char *filename)
{
        int fd = 0;
        autofree(xmlTextReader) *reader = NULL;
        bool ret = false;
        int r = 0;
        Ls2DTileSheetTSX parser = { 0 };

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
                ls2d_tile_sheet_walk_tsx(self, &parser, reader);
        }
        ret = true;

fail:
        if (fd >= 0) {
                close(fd);
        }

        return ret;
}

static void ls2d_tile_sheet_image_tsx(Ls2DTileSheet *self, Ls2DTileSheetTSX *parser,
                                      xmlTextReader *reader)
{
        autofree(xmlChar) *source = NULL;
        Ls2DTextureHandle handle;
        Ls2DTextureHandle subhandle;
        Ls2DTileSheetCell *cell = NULL;

        ls2d_tile_sheet_get_int_attr(reader, &parser->image.width, "width");
        ls2d_tile_sheet_get_int_attr(reader, &parser->image.height, "height");

        source = xmlTextReaderGetAttribute(reader, BAD_CAST "source");

        handle = ls2d_texture_cache_load_file(self->cache, (char *)source);

        /* Basic sheet logic. */
        if (!parser->sheet) {
                fprintf(stderr, "Put sheet\n");
                ls_array_add(self->texture_objs, NULL);
                cell = ls2d_tile_sheet_get_cell(self->texture_objs->data, parser->tile.id);
                cell->handle = handle;
                return;
        }

        int x = parser->tileset.margin;
        int y = parser->tileset.margin;
        int column = 0;

        for (int tile = 0; tile < parser->tileset.count; tile++) {
                SDL_Rect region = { .x = x,
                                    .y = y,
                                    .w = parser->tileset.width - parser->tileset.spacing,
                                    .h = parser->tileset.height - parser->tileset.spacing };

                /* seek */
                column++;

                if (column == parser->tileset.columns) {
                        x = parser->tileset.margin;
                        y += parser->tileset.spacing;
                        y += parser->tileset.height;
                        column = 0;
                } else {
                        x += parser->tileset.spacing;
                        x += parser->tileset.width;
                }

                subhandle = ls2d_texture_cache_subregion(self->cache, handle, region);
                if (!ls_array_add(self->texture_objs, NULL)) {
                        abort();
                }
                cell = ls2d_tile_sheet_get_cell(self->texture_objs->data, tile);
                cell->handle = subhandle;
                fprintf(stderr,
                        "GID %d to %d (of %d)\n",
                        tile,
                        cell->handle,
                        self->texture_objs->len);
        }
}

/**
 * Walk each node and then process it.
 */
static void ls2d_tile_sheet_walk_tsx(Ls2DTileSheet *self, Ls2DTileSheetTSX *parser,
                                     xmlTextReader *reader)
{
        const xmlChar *name = NULL;

        name = xmlTextReaderConstName(reader);
        if (!name) {
                return;
        }

        if (parser->in_tileset && xmlStrEqual(name, BAD_CAST "grid")) {
                parser->in_grid = !parser->in_grid;
                return;
        }

        if (parser->in_tileset && xmlStrEqual(name, BAD_CAST "tile")) {
                parser->in_tile = !parser->in_tile;
                if (!parser->in_tile) {
                        return;
                }
                ls2d_tile_sheet_get_int_attr(reader, &parser->tile.id, "id");
                return;
        }

        if (parser->in_tile && xmlStrEqual(name, BAD_CAST "image")) {
                parser->in_image = !parser->in_image;
                if (!parser->in_image) {
                        return;
                }
                ls2d_tile_sheet_image_tsx(self, parser, reader);
                return;
        } else if (parser->in_tileset && xmlStrEqual(name, BAD_CAST "image")) {
                parser->in_image = !parser->in_image;
                parser->sheet = !parser->sheet;
                if (!parser->in_image) {
                        return;
                }
                ls2d_tile_sheet_image_tsx(self, parser, reader);
                return;
        }

        if (parser->in_tile && xmlStrEqual(name, BAD_CAST "animation")) {
                parser->in_animation = !parser->in_animation;
                if (parser->in_animation) {
                        ls2d_tile_sheet_start_animation(self, parser);
                } else {
                        ls2d_tile_sheet_end_animation(self, parser);
                }
                return;
        }

        if (parser->in_animation && xmlStrEqual(name, BAD_CAST "frame")) {
                parser->in_frame = !parser->in_frame;
                ls2d_tile_sheet_add_frame(self, parser, reader);
                return;
        }

        if (xmlStrEqual(name, BAD_CAST "tileset")) {
                parser->in_tileset = !parser->in_tileset;
                if (!parser->in_tileset) {
                        return;
                }
                ls2d_tile_sheet_get_int_attr(reader, &parser->tileset.width, "tilewidth");
                ls2d_tile_sheet_get_int_attr(reader, &parser->tileset.height, "tileheight");
                ls2d_tile_sheet_get_int_attr(reader, &parser->tileset.spacing, "spacing");
                ls2d_tile_sheet_get_int_attr(reader, &parser->tileset.margin, "margin");
                ls2d_tile_sheet_get_int_attr(reader, &parser->tileset.count, "tilecount");
                ls2d_tile_sheet_get_int_attr(reader, &parser->tileset.columns, "columns");

                self->texture_objs =
                    ls_array_new_size(sizeof(Ls2DTileSheetCell), parser->tileset.count);
                /* TODO: Ensure this is created! */
        }
}

static void ls2d_tile_sheet_start_animation(Ls2DTileSheet *self, Ls2DTileSheetTSX *parser)
{
        if (parser->animation) {
                ls2d_animation_unref(parser->animation);
        }
        parser->animation = ls2d_animation_new();
}

static void ls2d_tile_sheet_end_animation(Ls2DTileSheet *self, Ls2DTileSheetTSX *parser)
{
        Ls2DTileSheetCell *cell = NULL;

        fprintf(stderr, "Pushing animation for tile %d\n", parser->tile.id);

        cell = ls2d_tile_sheet_get_cell(self->texture_objs->data, parser->tile.id);
        cell->animation = parser->animation;
        ls_array_add(self->animations, parser->animation);
        parser->animation = NULL;
}

static void ls2d_tile_sheet_add_frame(Ls2DTileSheet *self, Ls2DTileSheetTSX *parser,
                                      xmlTextReader *reader)
{
        Ls2DTileSheetCell *old_cell = NULL;
        Ls2DTileSheetCell *source_cell = NULL;

        int tile_id = 0;
        int duration = 0;

        ls2d_tile_sheet_get_int_attr(reader, &tile_id, "tileid");
        ls2d_tile_sheet_get_int_attr(reader, &duration, "duration");

        source_cell = ls2d_tile_sheet_get_cell(self->texture_objs->data, tile_id);
        old_cell = ls2d_tile_sheet_get_cell(self->texture_objs->data, parser->tile.id);

        if (!ls2d_animation_add_frame(parser->animation, source_cell->handle, (uint32_t)duration)) {
                abort();
        }

        fprintf(stderr,
                "Cell ID %d: Animate to %d at %d: %d\n",
                parser->tile.id,
                tile_id,
                duration,
                source_cell->handle);
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
