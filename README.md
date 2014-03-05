Tetris3d (aka TetrisV)
======================

A 3d cross-platform implementation of a game which remotely resembles tetris

## Contact ##

Email: a.baimagambetov1@uni.brighton.ac.uk

## Dependencies ##

* **[SDL](http://www.libsdl.org/)** **2.0.0**+ (including SDL_image, SDL_ttf, SDL_net)
* **OpenGL 2.1**+ (Preferably 3.0+)
* **[GLEW](http://glew.sourceforge.net/) 1.6**+

## Compile and run ##

For Linux:

`$ autoreconf -i`

`$ ./configure`

`$ make`

`$ ./TetrisV`

For Windows:

`TODO: MS Visual Studio Project will be here soon`
    
## Extra: ##

There's an android app that works as a wireless controller. Source code:
https://github.com/AlmasB/tetris3d-controller
    
## Known issues & bugs ##

* On linux VMs (Fedora20x64, Ubuntu13.10x64) with OpenGL 2.1 emulation
    only the front faces are drawn (nothing to do with glFaceCull though)