bin_PROGRAMS = TetrisV
TetrisV_SOURCES = src/game/Main.cpp src/game/Player.cpp src/game/Level.cpp src/game/GameObject.cpp src/game/Game.cpp
TetrisV_SOURCES += src/engine/audio/AudioEngine.cpp
TetrisV_SOURCES += src/engine/core/GameEngine.cpp src/engine/core/Debug.cpp src/engine/core/ResourceManager.cpp src/engine/core/Timer.cpp
TetrisV_SOURCES += src/engine/graphics/GraphicsEngine.cpp src/engine/graphics/Primitives3d.cpp src/engine/graphics/MD3Object.cpp src/engine/graphics/ShaderManager.cpp src/engine/graphics/Movable.cpp
TetrisV_SOURCES += src/engine/input/EventEngine.cpp
TetrisV_SOURCES += src/engine/physics/PhysicsEngine.cpp


AM_CPPFLAGS = -std=c++0x
AM_CPPFLAGS += $(gl_CFLAGS) $(glew_CFLAGS) $(sdl2_CFLAGS)
LIBS += $(gl_LIBS) $(glew_LIBS) $(sdl2_LIBS) $(SDL2_image_LIBS) $(SDL_ttf_LIBS) $(SDL_net_LIBS) $(SDL2_mixer_LIBS)
