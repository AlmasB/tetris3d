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
Windows 7/8/8.1 (x64)
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
External libs needed to build on Windows can be found under libs/
Simply point cmake to that folder, dlls are also located there

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

## Credits: ##

All resources shown below were free of charge and free to use on the date they were obtained
A "clong" sound effect - http://www.pacdv.com/sounds/index.html
"Tetris" font - http://www.1001freefonts.com/
"Brick" texture - http://www.cgtextures.com/

    
## Known issues & bugs ##

* With NVIDIA driver 311.48 (other versions may be affected too) SDL_GL_SetSwapInterval(1)
    (double buffering) causes high cpu load. Possibly NVIDIA driver issue or SDL lib bug, 
    other drivers not affected
    
* If you are using VirtualBox's machines, mouse behaves very strangely and it becomes unplayable.
    Possibly mouse handling in VMs differs from native handling.
    Use arrow keys instead if this is the case. This issue hasn't been seen on OSes running natively.
    
## Version ##

Current stable version is 0.2
Latest build version is 0.2.1
    
## Updates ##

Next stable version (0.3) will include:

* Powerups

* You will no longer die if you couldn't catch blocks in time. The blocks will disappear and another batch will be spawned.
    This will cost you points however.
    
* The player will be able to adjust mouse sensitivity in the game

* The player will be able to switch to fullscreen mode and back.
    (The feature already exists in build 0.2.1)