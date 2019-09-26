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

/**
 * This is the main inclusion header for lispysnake2d and will take care
 * of including any component headers.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Ls2DInputManager Ls2DInputManager;
typedef struct Ls2DEngine Ls2DEngine;
typedef struct Ls2DComponent Ls2DComponent;
typedef struct Ls2DEntity Ls2DEntity;
typedef struct Ls2DFrameInfo Ls2DFrameInfo;
typedef struct Ls2DObject Ls2DObject;
typedef struct Ls2DScene Ls2DScene;

typedef uint16_t Ls2DTextureHandle;
typedef struct Ls2DTextureCache Ls2DTextureCache;
typedef struct Ls2DTextureNode Ls2DTextureNode;

#include "libls.h"
#include "object.h"

#include "component.h"
#include "engine.h"
#include "entity.h"
#include "frame.h"
#include "input-manager.h"
#include "scene.h"
#include "texture-cache.h"

/* Our components */
enum Ls2DComponentID {
        LS2D_COMP_ID_INVALID = 0,
        LS2D_COMP_ID_POSITION = 1,
        LS2D_COMP_ID_SPRITE = 2,
};

#include "components/position.h"
#include "components/sprite.h"

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
