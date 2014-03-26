Tetris3d (aka TetrisV)
======================

A 3d cross-platform implementation of a game which remotely resembles tetris

## Contact ##

Email: a.baimagambetov1@uni.brighton.ac.uk

## Dependencies ##

* **[SDL](http://www.libsdl.org/)** **2.0.0**+ (including SDL_image, SDL_ttf, SDL_net, SDL_mixer)
* **OpenGL 2.1**+
* **[GLEW](http://glew.sourceforge.net/) 1.6**+

## Install ##

In dist/ you'll find installers for different platforms

## Compile and run from source code ##

For Linux:

`$ autoreconf -i`

`$ ./configure`

`$ make`

`$ ./TetrisV`

For Windows:

Use cmake 2.6+, CMakeLists.txt included
Use MS Visual Studio 2013+, as versions below don't support some c++ features used in Tetris3d

## Debug ##

In the file src/engine/core/Debug.h

uncomment #define __DEBUG to see hopefully useful debug info on terminal
    
## Extra: ##

There's an android app that works as a wireless controller. Source code:
https://github.com/AlmasB/tetris3d-controller
    
## Known issues & bugs ##

* With NVIDIA driver 311.48 (other versions may be affected too) SDL_GL_SetSwapInterval(1)
    (double buffering) causes high cpu load