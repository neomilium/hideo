#                                               -*- Autoconf -*-
#
# Copyright (c) 2004,  Theodore A. Roth
# Copyright (c) 2005,2006,2007,2009  Anatoly Sokolov
# Copyright (c) 2005,2008  Joerg Wunsch
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in
#   the documentation and/or other materials provided with the
#   distribution.
# * Neither the name of the copyright holders nor the names of
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
#
# Process this file with autoconf to produce a configure script.
#

dnl library versioning -- modify these on releases
dnl        macro name           value            description
m4_define([hideo_major],    [0])dnl           major version
m4_define([hideo_minor],    [9])dnl           minor version
m4_define([hideo_revision], [1])dnl           revision (dot-dot version)
m4_define([hideo_reldate],  [20090514])dnl    release date
dnl end of library versioning data

dnl odd minor number marks a development branch, append date to version there
m4_if(m4_eval(hideo_minor % 2 == 1), [0], [dnl
  m4_define([hideo_version],
            hideo_major.hideo_minor.hideo_revision)dnl
  ],[dnl
  m4_define([hideo_version],
            hideo_major.hideo_minor.hideo_revision.hideo_reldate)dnl
  ])dnl
m4_define([hideo_version_numeric],
          m4_eval(10000 * hideo_major + dnl
                    100 * hideo_minor + dnl
                          hideo_revision))dnl

AC_INIT(HiDeO, hideo_version, hideo@hideo.org)

AC_PREREQ(2.59)
AC_REVISION($Revision: 1.86 $)

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADER([src/config.h])

dnl substitute M4 macros into shell variables
HIDEO_MAJOR=hideo_major
HIDEO_MINOR=hideo_minor
HIDEO_REVISION=hideo_revision
HIDEO_RELDATE=hideo_reldate
HIDEO_VERSION=hideo_version
HIDEO_VERSION_NUMERIC=hideo_version_numeric
dnl ...and trigger Makefile.in substitutions
AC_SUBST(HIDEO_MAJOR)
AC_SUBST(HIDEO_MINOR)
AC_SUBST(HIDEO_REVISION)
AC_SUBST(HIDEO_RELDATE)
AC_SUBST(HIDEO_VERSION)
AC_SUBST(HIDEO_VERSION_NUMERIC)

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

# See if the user specified '--build=`../config.guess` --host=avr' when
# running ../configure.

AC_MSG_CHECKING([if configuring for cross compile])
if test "x${build_alias}" = "x${host_alias}"; then
	if test "x${build_alias}" = "x" ; then
		build_alias='`./config.guess`'
	fi
	AC_MSG_RESULT(no)
	AC_MSG_WARN()
	AC_MSG_WARN(HiDeO must be built using an avr cross-compiler.)
	AC_MSG_WARN(Try configuring with:)
	AC_MSG_WARN("$0 --build=$build_alias --host=avr")
	AC_MSG_WARN()
	AC_MSG_ERROR(aborting configure)
else
	AC_MSG_RESULT([yes])
fi

AC_MSG_CHECKING([if target host is avr])
case "$host" in
	avr* )
		AC_MSG_RESULT([yes])
		;;
	* )
		AC_MSG_RESULT(no)
		AC_MSG_WARN()
		AC_MSG_WARN(HiDeO must be built using an avr cross-compiler.)
		AC_MSG_WARN(Try configuring with:)
		AC_MSG_WARN("$0 --build=$build --host=avr")
		AC_MSG_WARN()
		AC_MSG_ERROR(aborting configure)
		;;
esac

## TODO: Write a check for GNU Make

dnl The default check whether the C compiler can create an executable
dnl is inappropriate for us as it requires an existing library.
# AC_NO_EXECUTABLES

dnl Minimum version of Automake is 1.8.
dnl We don't want the gzip distribution tarball anymore.
AM_INIT_AUTOMAKE([1.8 dist-bzip2 no-dist-gzip])

# We don't want touse the cflags from the environment since we need control
# of this when we're building the firmware.
CFLAGS="-funsigned-bitfields -fpack-struct -fshort-enums -std=gnu99 -mmcu=atmega32 -mcall-prologues -Wall -Os -DF_CPU=8000000UL"

# Checks for programs.

AC_PROG_CC
AC_CHECK_TOOL(AS, as, as)
AM_PROG_AS
# AC_PROG_RANLIB
AC_CHECK_TOOL(AR, ar, ar)

# Make sure that we found the right avr cross-compiler.

case "${CC}" in
   *avr-gcc*) ;;
   *) AC_MSG_ERROR(Wrong C compiler found; check the PATH!) ;;
esac
case "${AS}" in
   *avr-as*) ;;
   *) AC_MSG_ERROR(Wrong assembler found; check the PATH!) ;;
esac
case "${AR}" in
   *avr-ar*) ;;
   *) AC_MSG_ERROR(Wrong archiver found; check the PATH!) ;;
esac

# case "${RANLIB}" in
#    *avr-ranlib*) ;;
#    *) AC_MSG_ERROR(Wrong ranlib found; check the PATH!) ;;
# esac

# AC_PROG_INSTALL
# AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl TODO: Check for various tools used to build the documentation. If anything
dnl is missing, don't build the docs by default.

dnl Disable building of dox by default since it they may not build properly on
dnl all systems. This isn't optimal, but how do we work around the user not
dnl having fig2dev and still build all the formats?

# dnl
# dnl @synopsis CHECK_PNG_UTILS
# dnl
# dnl This macro checks if pngtopnm and pnmtopng are installed. If they are not
# dnl installed we just fake it with "cat".
# dnl
# dnl We use these in the dox to insert transparency into the png images as such:
# dnl
# dnl   $ pngtopnm foo.png > tmp.pnm
# dnl   $ pnmtopng -transparent white tmp.pnm > foo.png
# dnl
# dnl @version 
# dnl @author Theodore A. Roth <troth@openavr.org> and Eric B. Weddington <eweddington@cso.atmel.com>
# dnl
# AC_DEFUN([CHECK_PNG_UTILS],[dnl
# dnl
# AC_MSG_CHECKING([for pngtopnm])
# has_pngtopnm=`pngtopnm --version 2>&1 | grep -c -i Version`
# if test "$has_pngtopnm" = "1"; then
# 	AC_MSG_RESULT(yes)
# 	PNGTOPNM="pngtopnm"
# else
# 	AC_MSG_RESULT(no)
# 	PNGTOPNM="cat"
# fi
# dnl
# AC_MSG_CHECKING([for pnmtopng])
# has_pnmtopng=`pnmtopng --version 2>&1 | grep -c -i Version`
# if test "$has_pnmtopng" = "1"; then
# 	AC_MSG_RESULT(yes)
# 	PNMTOPNG="pnmtopng"
# else
# 	AC_MSG_RESULT(no)
# 	PNMTOPNG="cat"
#     PNGTOPNM="cat"
# fi
# AC_SUBST(PNGTOPNM)
# AC_SUBST(PNMTOPNG)
# ])dnl
# 
# CHECK_PNG_UTILS

FNO_JUMP_TABLES=""
AC_SUBST(FNO_JUMP_TABLES)

AC_DEFUN(
  CHECK_MNO_TABLEJUMP,
  [
    old_CC=${CC}
    old_CFLAGS=${CFLAGS}
    CFLAGS="-mno-tablejump"
    CC=`echo "${CC}" | sed 's/-mmcu=avr.//'`
    AC_MSG_CHECKING(whether ${CC} supports -mno-tablejump)
    AC_COMPILE_IFELSE(
      [AC_LANG_SOURCE([],[])],
      [has_mno_tablejump=yes],
      [has_mno_tablejump=no]
    )
    if test "x$has_mno_tablejump" = "xyes"
    then
      FNO_JUMP_TABLES="-mno-tablejump"
    fi
    AC_MSG_RESULT($has_mno_tablejump)
    CC=${old_CC}
    CFLAGS=${old_CFLAGS}
  ]
)

AC_DEFUN(
  CHECK_FNO_JUMP_TABLES,
  [
    old_CC=${CC}
    old_CFLAGS=${CFLAGS}
    CFLAGS="-fno-jump-tables"
    CC=`echo "${CC}" | sed 's/-mmcu=avr.//'`
    AC_MSG_CHECKING(whether ${CC} supports -fno-jump-tables)
    AC_COMPILE_IFELSE(
      [AC_LANG_SOURCE([],[])],
      [has_fno_jump_tables=yes],
      [has_fno_jump_tables=no]
    )
    if test "x$has_fno_jump_tables" = "xyes"
    then
      FNO_JUMP_TABLES="-fno-jump-tables"
    fi
    AC_MSG_RESULT($has_fno_jump_tables)
    CC=${old_CC}
    CFLAGS=${old_CFLAGS}
  ]
)

CHECK_MNO_TABLEJUMP
CHECK_FNO_JUMP_TABLES

dnl Some devices are only handled by newer version of gcc. This macro lets us
dnl probe to see if the installed avr-gcc supports a questionable device.
AC_DEFUN(
  CHECK_AVR_DEVICE,
  [
    old_CC=${CC}
    old_CFLAGS=${CFLAGS}
    CFLAGS="-mmcu=$1"
    CC=`echo "${CC}" | sed 's/-mmcu=avr.//'`
    AC_MSG_CHECKING(if ${CC} has support for $1)
    AC_COMPILE_IFELSE(
      [AC_LANG_SOURCE([],[])],
      [has_dev_support=yes],
      [has_dev_support=no]
    )
    if test "x$has_dev_support" = "xyes"
    then
      HAS_$1=yes
    fi
    AC_MSG_RESULT($has_dev_support)
    CC=${old_CC}
    CFLAGS=${old_CFLAGS}
  ]
)

# avr5
AM_CONDITIONAL(HAS_avr5, true)
AM_CONDITIONAL(HAS_atmega32, true)

CHECK_AVR_DEVICE(atmega324p)
AM_CONDITIONAL(HAS_atmega324p, test "x$HAS_atmega324p" = "xyes")

CHECK_AVR_DEVICE(atmega325)
AM_CONDITIONAL(HAS_atmega325, test "x$HAS_atmega325" = "xyes")

CHECK_AVR_DEVICE(atmega325p)
AM_CONDITIONAL(HAS_atmega325p, test "x$HAS_atmega325p" = "xyes")

CHECK_AVR_DEVICE(atmega3250)
AM_CONDITIONAL(HAS_atmega3250, test "x$HAS_atmega3250" = "xyes")

CHECK_AVR_DEVICE(atmega3250p)
AM_CONDITIONAL(HAS_atmega3250p, test "x$HAS_atmega3250p" = "xyes")

CHECK_AVR_DEVICE(atmega328p)
AM_CONDITIONAL(HAS_atmega328p, test "x$HAS_atmega328p" = "xyes")

CHECK_AVR_DEVICE(atmega329)
AM_CONDITIONAL(HAS_atmega329, test "x$HAS_atmega329" = "xyes")

CHECK_AVR_DEVICE(atmega329p)
AM_CONDITIONAL(HAS_atmega329p, test "x$HAS_atmega329p" = "xyes")

CHECK_AVR_DEVICE(atmega3290)
AM_CONDITIONAL(HAS_atmega3290, test "x$HAS_atmega3290" = "xyes")

CHECK_AVR_DEVICE(atmega3290p)
AM_CONDITIONAL(HAS_atmega3290p, test "x$HAS_atmega3290p" = "xyes")

CHECK_AVR_DEVICE(atmega32hvb)
AM_CONDITIONAL(HAS_atmega32hvb, test "x$HAS_atmega32hvb" = "xyes")

# Generate all files from *.in sources.

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])

AC_OUTPUT
