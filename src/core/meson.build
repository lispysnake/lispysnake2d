# Build the core library portion

core_sources = [
     'component.c',
     'entity.c',
     'engine.c',
     'engine-draw.c',
     'engine-update.c',
     'object.c',
     'scene.c',
     'sprite.c',
     'tilesheet.c',
]

core_deps = [
    dep_sdl_core,
    dep_sdl_image,
    dep_xml,
    libls.get_variable('link_libls'),
]

libcore = static_library(
    'lispysnake2d_core',
    sources: core_sources,
    dependencies: core_deps,
    install: false,
)

link_libcore = declare_dependency(
    link_with: libcore,
    dependencies: core_deps,
    include_directories: include_directories('.'),
)
