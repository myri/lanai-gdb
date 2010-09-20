dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)dnl
AC_INIT(Makefile.in)
AC_CONFIG_HEADER(config.h:config.in)

dnl --with-lanaiqdir=/some/path
AC_ARG_WITH([lanaiqdir],
  [AS_HELP_STRING([--with-lanaiqdir],[where to find LanaiQ sources])])
dnl This arranges to replace @LANAIQDIR@ in Makefile.in
AC_SUBST([LANAIQDIR],$with_lanaiqdir)  

sinclude(../common/aclocal.m4)

# Bugs in autoconf 2.59 break the call to SIM_AC_COMMON, hack around
# it by inlining the macro's contents.
sinclude(../common/common.m4)

SIM_AC_OPTION_WARNINGS

AC_CHECK_HEADERS(unistd.h)


SIM_AC_OUTPUT
