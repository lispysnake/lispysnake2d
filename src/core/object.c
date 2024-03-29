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
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#include "ls2d.h"

void *ls2d_object_ref(void *v)
{
        Ls2DObject *object = v;
        if (!v) {
                return NULL;
        }
        int ref_count = atomic_fetch_add(&(object->ref_count), 1);
        assert(ref_count >= 0);
        return v;
}

void *ls2d_object_unref(void *v)
{
        Ls2DObject *object = v;
        if (!v) {
                return NULL;
        }
        int ref_count = atomic_fetch_sub(&(object->ref_count), 1);
        assert(ref_count > 0);
        if (ref_count == 1) {
                printf("Killing object %s* %p\n", object->vtable->obj_name, (void *)object);
                if (object->vtable->destroy) {
                        object->vtable->destroy(v);
                } else {
                        free(object);
                }
                return NULL;
        }
        return v;
}

void *ls2d_object_init(Ls2DObject *object, Ls2DObjectTable *vtable)
{
        assert(object != NULL);
        assert(vtable != NULL);
        object->ref_count = ATOMIC_VAR_INIT(1);
        object->vtable = vtable;
        if (ls_unlikely(!object->vtable->init)) {
                return object;
        }
        printf("Init object %s* %p\n", object->vtable->obj_name, (void *)object);
        /* TODO: Ensure init passes..? */
        object->vtable->init(object);
        return object;
}

Ls2DObject *ls2d_object_new(size_t size, Ls2DObjectTable *vtable)
{
        void *obj = NULL;

        assert(size > sizeof(struct Ls2DObject));
        obj = calloc(1, size);
        if (!obj) {
                fprintf(stderr, "Failed to allocate object %s\n", vtable->obj_name);
                return NULL;
        }
        return ls2d_object_init((Ls2DObject *)obj, vtable);
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
