#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(elfo, 0.5.0RC2, thegeek@thegeekinside.net)
AM_INIT_AUTOMAKE(elfo, 0.5.0RC2)
AC_CONFIG_SRCDIR([src/elfo.c])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_STAT
AC_FUNC_UTIME_NULL
AC_CHECK_FUNCS([utime])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
