cmake_minimum_required (VERSION 2.6)
project (Tetris3)
find_package(OpenGL REQUIRED)

set(EXE ${PROJECT_NAME})
set(SRC_DIR src)
set(ENGINE_DIR ${SRC_DIR}/engine)
set(GAME_DIR ${SRC_DIR}/game)

# get all src and headers
file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp ${SRC_DIR}/*.h)

# add include directories 
include_directories(libs/SDL2.0/include/)
include_directories(libs/glew-1.10.0/include/)

# add library search paths 
link_directories(libs/SDL2.0/lib/)
link_directories(libs/glew-1.10.0/lib/Release/Win32/)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(Tetris3 ${SRC_FILES})

# link against libraries
target_link_libraries(${EXE} debug SDL2.lib)
target_link_libraries(${EXE} optimized SDL2.lib)
target_link_libraries(${EXE} debug SDL2main.lib)
target_link_libraries(${EXE} optimized SDL2main.lib)
target_link_libraries(${EXE} debug SDL2_image.lib)
target_link_libraries(${EXE} optimized SDL2_image.lib)
target_link_libraries(${EXE} debug SDL2_mixer.lib)
target_link_libraries(${EXE} optimized SDL2_mixer.lib)
target_link_libraries(${EXE} debug SDL2_ttf.lib)
target_link_libraries(${EXE} optimized SDL2_ttf.lib)
target_link_libraries(${EXE} debug SDL2_net.lib)
target_link_libraries(${EXE} optimized SDL2_net.lib)

target_link_libraries(${EXE} ${OPENGL_LIBRARY})
target_link_libraries(${EXE} debug glew32.lib)
target_link_libraries(${EXE} optimized glew32.lib)
