# Build the core library portion

core_sources = [
     'engine.c',
]

core_deps = [
    dep_sdl_core,
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