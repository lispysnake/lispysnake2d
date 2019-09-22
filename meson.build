project(
    'lispysnake2d',
    ['c'],
    version: '0.0.0',
    license: [
        'ZLib',
    ],
    default_options: [
        'c_std=c11',
        'prefix=/usr',
        'sysconfdir=/etc',
        'localstatedir=/var',
    ],
)

am_cflags = [
    '-fstack-protector',
    '-Wall',
    '-pedantic',
    '-Wstrict-prototypes',
    '-Wundef',
    '-fno-common',
    '-Werror-implicit-function-declaration',
    '-Wformat',
    '-Wformat-security',
    '-Werror=format-security',
    '-Wconversion',
    '-Wunused-variable',
    '-Wunreachable-code',
    '-W',
]

add_global_arguments(am_cflags, language: 'c')

# Compute dependencies
dep_sdl_core = dependency('sdl2', version: '>= 2.0.9')
dep_sdl_image = dependency('SDL2_image', version: '>= 2.0.4')

# We need libxml2 for format parsers
dep_xml = dependency('libxml-2.0', version: '>= 2.9.4')

# Get configuration bits together
path_prefix = get_option('prefix')
path_sysconfdir = join_paths(path_prefix, get_option('sysconfdir'))

# Sort out config.h now
cdata = configuration_data()

# General options..
cdata.set_quoted('PACKAGE_NAME', meson.project_name())
cdata.set_quoted('PACKAGE_VERSION', meson.project_version())

# Build our convenience library..
libls = subproject(
    'libls',
    default_options: [
        'c_std=c11',
        'with-tests=false',
        'with-static=true',
    ]
)

# Now build bits
subdir('src')

report = [
    '    Build configuration:',
    '    ====================',
    '',
    '    prefix:                                 @0@'.format(path_prefix),
    '    sysconfdir:                             @0@'.format(path_sysconfdir),
]

if meson.is_subproject() == false
    # Output some stuff to validate the build config
    message('\n\n\n' + '\n'.join(report) + '\n\n')
endif
