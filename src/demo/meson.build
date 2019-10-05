demo_sources = [
    'main.c',
    'level.c',
]

demo_dependencies = [
    link_libcore,
]

demo = executable(
     'lispysnake2d_demo',
     sources: demo_sources,
     c_args: am_cflags,
     dependencies: demo_dependencies,
     install: false,
)
