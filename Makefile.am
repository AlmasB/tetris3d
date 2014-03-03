bin_PROGRAMS = TetrisV
TetrisV_SOURCES = src/Main.cpp src/engine/Primitives3d.cpp src/engine/BoundingBox.cpp src/engine/EventEngine.cpp src/Game.cpp src/engine/GameEngine.cpp src/GameObject.cpp src/engine/GraphicsEngine.cpp src/Level.cpp src/engine/Movable.cpp src/Player.cpp src/engine/ResourceManager.cpp src/engine/Timer.cpp

AM_CPPFLAGS = -std=c++11
AM_CPPFLAGS += $(gl_CFLAGS) $(glew_CFLAGS) $(sdl2_CFLAGS)
LIBS += $(gl_LIBS) $(glew_LIBS) $(sdl2_LIBS) $(SDL2_image_LIBS) $(SDL2_ttf_LIBS) $(SDL2_net_LIBS)
