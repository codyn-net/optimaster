AC_INIT(optimaster, 2.3)

AM_INIT_AUTOMAKE([subdir-objects])

AC_CONFIG_MACRO_DIR([m4])
AC_PROG_LIBTOOL

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_ISC_POSIX
AC_STDC_HEADERS
AC_PROG_CXX
AC_HEADER_STDC

AC_PROG_INSTALL
AC_PROG_MAKE_SET

AC_CHECK_LIB([gcrypt],[gcry_check_version],[],[AC_MSG_ERROR(Could not find gcrypt)])

AC_ARG_ENABLE([docs],
	AS_HELP_STRING([--enable-docs],[Enable documentation building (default: disabled)]),
	[enable_docs=$enableval],
	[enable_docs=no])

if test "x$enable_docs" != "xno"; then
	AC_CHECK_PROG(DOXYGEN, doxygen, doxygen, no)
	
	if test x"$DOXYGEN" == "xno"; then
		AC_MSG_ERROR([you need to have doxygen installed for generating documentation])
	fi
fi

AM_CONDITIONAL(ENABLE_DOCS, test x"$enable_docs" = "xyes")

AC_CONFIG_HEADERS(config.h)
AM_MAINTAINER_MODE

PKG_CHECK_MODULES(PACKAGE_OPTIMASTER, [
	liboptimization-2.0 >= 2.2
])

if test "x$USE_MAINTAINER_MODE" = "xyes"; then
PACKAGE_OPTIMASTER_CFLAGS="$PACKAGE_OPTIMASTER_CFLAGS -Wall -Werror -Wuninitialized -Wmissing-declarations"
fi

AC_SUBST(PACKAGE_OPTIMASTER_CFLAGS)
AC_SUBST(PACKAGE_OPTIMASTER_LIBS)

AC_OUTPUT(
Makefile
src/Makefile
docs/Makefile
docs/doxygen.cfg
)
