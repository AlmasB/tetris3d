bin_PROGRAMS = TetrisV
TetrisV_SOURCES = src/Main.cpp src/BoundingBox.cpp src/EventEngine.cpp src/Game.cpp src/GameEngine.cpp src/GameObject.cpp src/GraphicsEngine.cpp src/Level.cpp src/Movable.cpp src/Player.cpp src/ResourceManager.cpp src/Timer.cpp

AM_CPPFLAGS = -std=c++11
AM_CPPFLAGS += $(gl_CFLAGS) $(glew_CFLAGS) $(sdl2_CFLAGS)
LIBS += $(gl_LIBS) $(glew_LIBS) $(sdl2_LIBS) $(SDL2_image_LIBS)
