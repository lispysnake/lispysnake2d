# Build the core library portion

core_sources = [
     'animation.c',
     'camera.c',
     'component.c',
     'engine.c',
     'engine-draw.c',
     'engine-update.c',
     'entity.c',
     'input-manager.c',
     'object.c',
     'scene.c',
     'texture-cache.c',
     'tilesheet/sheet.c',
     'tilesheet/tsx.c',
     'components/animation-component.c',
     'components/position.c',
     'components/sprite.c',
     'entities/basic-entity.c',
     'entities/image.c',
     'entities/tilemap.c',
     'entities/tilemap-tmx.c',
     'spritesheet/sheet.c',
     'spritesheet/xml.c',
]

core_deps = [
    dep_sdl_core,
    dep_sdl_image,
    dep_xml,
    libls.get_variable('link_libls'),
    meson.get_compiler('c').find_library('m', required: false),
]

libcore = static_library(
    'lispysnake2d_core',
    sources: core_sources,
    c_args: am_cflags,
    dependencies: core_deps,
    install: false,
)

link_libcore = declare_dependency(
    link_with: libcore,
    dependencies: core_deps,
    include_directories: include_directories('.'),
)
