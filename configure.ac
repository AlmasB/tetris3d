AC_INIT([TetrisV], [0.2])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_SRCDIR([configure.ac])
AC_PROG_CXX

AC_CANONICAL_HOST
AC_MSG_CHECKING([operating system])

case $host in
     *-darwin* ) LIBS="${LIBS} -framework OpenGL" ;;
     * ) macosx=no ;;
esac

# Find the following libraries on the system

AS_IF([test "x$macosx" = "xno"], [
   PKG_CHECK_MODULES([gl], [gl])
])

PKG_CHECK_MODULES([glew], [glew])
PKG_CHECK_MODULES([sdl2], [sdl2])
PKG_CHECK_MODULES([SDL2_image], [SDL2_image])
PKG_CHECK_MODULES([SDL2_ttf], [SDL2_ttf])
PKG_CHECK_MODULES([SDL2_net], [SDL2_net])
PKG_CHECK_MODULES([SDL2_mixer], [SDL2_mixer])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
