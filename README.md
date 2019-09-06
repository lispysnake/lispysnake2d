## lispysnake2d

A simple, lightweight 2D engine for [Lispy Snake, Ltd.](https://lispysnake.com/) graphical applications.
Provides basic SDL wrapping facilities to create cross-platform 2D games
and applications.

For now, entry-point hooks are not provided, i.e NDK/JNI.

### Design Considerations

Right now, no API or logic actually exists, so stand by while we flesh it out.
Below is a basic list of considerations in the design:

 - Build as either a shared or static library
 - Be a well-behaved submodule for meson builds
 - Platform agnosic where possible.
 - Modern C standards/design
 - Initially some loader-style functions to split out into a util lib in future.
 - Provide pure-2D and isometric engine styles
 - Basic engine tick management
 - Simple animation handler
 - Sprite notion
 - Super basic OOP approach.
 - Input handling system (event dispatch)
 - Only support a single window system. (vsync/multiple windows = complex)

### License

`lispysnake2d` is provided under the terms of the `ZLib` license.
Copyright © 2019 Lispy Snake, Ltd.
