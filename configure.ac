AC_PREREQ([2.60])

AC_INIT([criterion], [1.2.1], [], [criterion], [franklinmathieu@gmail.com])
AC_CONFIG_SRCDIR([src/runner.c])

LT_PREREQ([2.2.4])
AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE([dist-bzip2 no-dist-gzip foreign subdir-objects parallel-tests color-tests])
LT_INIT([disable-static])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AM_PROG_CC_C_O

AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S

AC_PROG_MAKE_SET
AC_SUBST([LIBTOOL_DEPS])

AC_CHECK_LIB([csptr], [smalloc], [], [AC_MSG_ERROR([Could not find libcsptr dependency. \
              Please go to https://github.com/Snaipe/c-smart-pointers.git \
              and follow the installation instructions.])])

enable_rt_tests="no"
AC_CHECK_LIB([rt], [clock_gettime], [enable_rt_tests="yes"])

AM_CONDITIONAL([ENABLE_RT_TESTS], [test "x$enable_rt_tests" != "xno"])

AC_CHECK_LIB([pcre], [pcre_compile], [AC_DEFINE([HAVE_PCRE], [1], [Define to 1 if you have the `pcre` library (-lpcre)])])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18])

AC_ARG_ENABLE([gcov],
  [AS_HELP_STRING([--enable-gcov],
    [Compile the project with converage enabled])],
    [COVERAGE_CFLAGS="-O0 -fprofile-arcs -ftest-coverage"
     COVERAGE_LDFLAGS="-lgcov"
     AC_SUBST([COVERAGE_CFLAGS])
     AC_SUBST([COVERAGE_LDFLAGS])
    ],
    [])

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile samples/Makefile po/Makefile.in])

AC_OUTPUT
