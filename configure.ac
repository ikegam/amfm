#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

PACKAGE=amfm
VERSION=0.1


AC_PREREQ([2.63])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([amfm.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE($PACKAGE, $VERSION, no-define)

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES(LIBHID, libhid >= 0.2.0)
AC_SUBST(LIBHID_CFLAGS)
AC_SUBST(LIBHID_LIBS)

# Checks for header files.
AC_CHECK_HEADERS([string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
