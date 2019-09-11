## lispysnake2d

A simple, lightweight 2D engine for [Lispy Snake, Ltd.](https://lispysnake.com/) graphical applications.
Provides basic SDL wrapping facilities to create cross-platform 2D games
and applications.

For now, entry-point hooks are not provided, i.e NDK/JNI.

### Design Considerations

 - Modern C standards
 - Initially implemented using SDL_Renderer
 - In future, convert to 3D rendering pipeline (more below)
 - Support multiple standard 2D perspectives
 - Support sprites, tiling, etc.
 - Support for animations
 - Tick based
 - Framerate limiting
 - Simple OOP design
 - Menu system

### Basic 2D Pipeline

Initially we'll have a simple 2D pipeline using SDL_Renderer, which will
be linked into the input system. Effectively, we'll have layers and
renderables within a scene to ensure we mask unnecessary input testing.

### Future 3D pipeline

Eventually we're going to swap out the pipeline for one that's 3D. This
will leverage the underlying SDL library to support various OpenGL profiles
and Vulkan. With this, we'll provide stock shaders, and allow implementing
custom ones. The API will largely remain the same but users will now get
a 3D pipeline for slick visual effects, whilst working in a 2D space.

### License

`lispysnake2d` is provided under the terms of the `ZLib` license.
Copyright © 2019 Lispy Snake, Ltd.
