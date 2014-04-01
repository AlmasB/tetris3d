Tetris3d (aka TetrisV)
======================

A 3d cross-platform implementation of a game which remotely resembles tetris

## Contact ##

Email: a.baimagambetov1@uni.brighton.ac.uk

## Dependencies ##

* **[SDL](http://www.libsdl.org/)** **2.0.0**+ (including SDL_image, SDL_ttf, SDL_net, SDL_mixer)
* **OpenGL 2.1**+
* **[GLEW](http://glew.sourceforge.net/) 1.6**+

For Fedora use yum, Ubuntu apt-get
For Mac OS use homebrew

SDL2 sub libraries can be built from source if the package is not available
for your platform http://www.libsdl.org/projects/

## Install ##

In dist/ you'll find installers for different platforms
Windows 7/8/8.1
Mac OS X

## Compile and run from source code ##

For Linux and Mac OS:

`$ autoreconf -i`

`$ ./configure`

`$ make`

`$ ./TetrisV`

For Windows:

Use cmake 2.6+, CMakeLists.txt included
Use MS Visual Studio 2013+, as versions below don't support some c++ features used in Tetris3d

## How to play ##

Use mouse or arrow keys to look around. LMB or SPACE to grab an object. After you grab use WSAD to
move the object. The direction is based on your camera, so this takes time to get used to but nice
part of gameplay, otherwise the game would be too easy

## Debug ##

In the file src/engine/core/Debug.h

uncomment #define __DEBUG to see hopefully useful debug info on terminal
    
## Extra: ##

There's an android app that works as a wireless controller. Source code:
https://github.com/AlmasB/tetris3d-controller
    
## Known issues & bugs ##

* With NVIDIA driver 311.48 (other versions may be affected too) SDL_GL_SetSwapInterval(1)
    (double buffering) causes high cpu load