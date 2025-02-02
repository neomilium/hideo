#! /bin/sh
#
# bootstrap script to build all the *.in files and configure script.
#

: ${AUTOHEADER="autoheader${AC_VER}"}
: ${AUTOCONF="autoconf${AC_VER}"}
: ${ACLOCAL="aclocal${AM_VER}"}
: ${AUTOMAKE="automake${AM_VER}"}

AUTOMAKE="${AUTOMAKE} --foreign --add-missing --copy"

export AUTOMAKE AUTOCONF ACLOCAL AUTOHEADER

# to see what is executed
set -x

rm -rf autom4te.cache

${ACLOCAL}
${AUTOHEADER}
${AUTOCONF}
${AUTOMAKE} 
