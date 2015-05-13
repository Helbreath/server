#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Helbreath Xtreme], [0.1], [zero@helbreathx.net])
AM_INIT_AUTOMAKE([no-define])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lPocoData':
AC_CHECK_LIB([PocoData], [main])
# FIXME: Replace `main' with a function in `-lPocoDataMySQL':
AC_CHECK_LIB([PocoDataMySQL], [main])
# FIXME: Replace `main' with a function in `-lPocoDataSQLite':
AC_CHECK_LIB([PocoDataSQLite], [main])
# FIXME: Replace `main' with a function in `-lPocoFoundation':
AC_CHECK_LIB([PocoFoundation], [main])
# FIXME: Replace `main' with a function in `-lPocoNet':
AC_CHECK_LIB([PocoNet], [main])
# FIXME: Replace `main' with a function in `-lPocoUtil':
AC_CHECK_LIB([PocoUtil], [main])
# FIXME: Replace `main' with a function in `-lboost_chrono':
AC_CHECK_LIB([boost_chrono], [main])

# Checks for header files.
AC_CHECK_HEADERS([memory.h stdint.h stdlib.h sys/timeb.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([ftime memset pow select socket sqrt])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
