AC_INIT([TetrisV], [0.1])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_SRCDIR([configure.ac])
AC_PROG_CXX # or AC_PROG_CXX

# Find the following libraries on the system
PKG_CHECK_MODULES([gl], [gl])
PKG_CHECK_MODULES([glew], [glew])
PKG_CHECK_MODULES([sdl2], [sdl2])
PKG_CHECK_MODULES([SDL2_image], [SDL2_image])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
