#!/usr/bin/env sh

THISDIR="$(dirname $0)"
test "$THISDIR" = "." && THISDIR=${PWD}

RESULT=$("${THISDIR}"/python-config.sh "$@")

exec_prefix_win=$("${THISDIR}"/python-config.sh --exec-prefix)
exec_prefix_unix=/c/mingw610/x86_64-610-posix-seh-rt_v5/mingw64/opt
echo $(echo $RESULT | sed "s#${exec_prefix_win}#${exec_prefix_unix}#g")
