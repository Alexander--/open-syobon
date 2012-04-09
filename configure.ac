#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([Open Syobon],
        [1.0.2],
        [https://github.com/Alexander--/open-syobon/issues],
        [open-syobon])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

# Creates Makefile.in
AM_INIT_AUTOMAKE([open-syobon], [1.0.2])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# Check for Windows
MINGW_AC_WIN32_NATIVE_HOST

# Check for packages
PKG_CHECK_MODULES([SDL], [sdl], [], [
  AC_MSG_ERROR([unable to find SDL; make sure it's development packages are installed!])
])

# Checks for libraries.
AC_CHECK_LIB([SDL_gfx], [pixelColor], [], [
  AC_MSG_ERROR([unable to find SDL_gfx, aborting!])
])
AC_CHECK_LIB([SDL_image], [IMG_Quit], [], [
  AC_MSG_ERROR([unable to find SDL_image, aborting!])
])
AC_CHECK_LIB([SDL_mixer], [Mix_OpenAudio], [], [
  AC_MSG_ERROR([unable to find SDL_mixer, aborting!])
])
AC_CHECK_LIB([SDL_ttf], [TTF_Quit], [], [
  AC_MSG_ERROR([unable to find SDL_ttf, aborting!])
])

# Optionally check for fontconfig
AC_ARG_WITH([fontconfig],
    AS_HELP_STRING([--without-fontconfig], [Ignore presence of fontconfig and disable it]))

AS_IF([test "x$with_fontconfig" != "xno"],
      [AC_CHECK_LIB([fontconfig], [FcPatternCreate], [have_fontconfig=yes], [have_fontconfig=no])],
      [have_fontconfig=no])

AS_IF([test "x$have_fontconfig" = "xyes"], [
            FONTCONFIG_FLAGS="$LDADD -lfontconfig"
            AC_DEFINE([USE_FONTCONFIG], [1], [Define if using fontconfig.])
            AH_TOP([#include "fontconfig/fontconfig.h"])
            ],
      [AS_IF([test "x$with_fontconfig" = "xyes"],
             [AC_MSG_ERROR([unable to find libfontconfig! Install it or build without --with-fontconfig option])
      ])
])

# Checks for header files.
AC_CHECK_HEADERS([libintl.h locale.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([setlocale])

# Checks for data placement location
AM_CONDITIONAL([PLATF_WINDOWS], [test "x$mingw_cv_win32_host" = "xyes"])

AC_SUBST([FONTCONFIG_FLAGS])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
