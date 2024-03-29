# Autoconf requirements

# Initialize, but use more options. Note parameters brackts and whitespace.
AC_INIT([STL], [1.0], [anton2920@gmail.com])

# Safety check - list a source file that wouldn't be in other directories:
AC_CONFIG_SRCDIR([../STL.h])

# Put configuration results here, so we can easily #include them:
AC_CONFIG_HEADERS([../STL_Config.h])

# Put autotools auxiliary files in a subdir, so they don't clutter top dir:
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# Enable «automake» to simplify creating makefile:
AM_INIT_AUTOMAKE
AC_CONFIG_FILES([Makefile])

# Look for a C compiler:
AC_PROG_CC

# Check for: (more) programs, libraries, header files, types,
# structures, compiler characteristics, library functions, system services.
LT_INIT

AC_ENABLE_SHARED
AC_PROG_LIBTOOL

# Check for essential libraries
AC_SEARCH_LIBS([malloc], [c], [], [
	AC_MSG_ERROR([unable to find c lib])
	])

AC_CHECK_HEADERS([stdio.h stdlib.h malloc.h], [], [
	AC_MSG_ERROR([unable to find necessary headers])
	])

 # Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Do final output.
AC_OUTPUT

echo 
echo " ---------------------------------"
echo "| Options used to compile and link:"
echo "| C             = $CС"
echo "| CFLAGS        = $CFLAGS"
echo "| LDFLAGS       = $LDFLAGS"
echo "| ARFLAGS       = $ARFLAGS"
echo " ---------------------------------"