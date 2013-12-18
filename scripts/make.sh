#!/bin/bash

MY_PWD=`pwd`

PROJECT_DIR="$(dirname $(dirname $(readlink -m $0)))"

function clean() {
    rm -Rf $PROJECT_DIR/build
    rm -Rf $PROJECT_DIR/pars
    rm -Rf $PROJECT_DIR/lib
    rm -Rf $PROJECT_DIR/include
    rm -Rf $PROJECT_DIR/tmp
    rm -Rf $PROJECT_DIR/.settings
    echo "==> Cleaning done"
}

[ "$1" = "clean" ] && clean && exit 0

[ "$1" = "recreate" ] && clean && shift

MY_ASTYLE_EXCLUDE=""
[ -d $PROJECT_DIR/build ] && MY_ASTYLE_EXCLUDE="--exclude=$PROJECT_DIR/build $MY_ASTYLE_EXCLUDE"
[ -d $PROJECT_DIR/include ] && MY_ASTYLE_EXCLUDE="--exclude=$PROJECT_DIR/include $MY_ASTYLE_EXCLUDE"

IS_ASTYLE=$(which astyle)
[ $? -eq 0 ] && astyle --options=$PROJECT_DIR/etc/astylerc --recursive $MY_ASTYLE_EXCLUDE *.h *.cxx *.C


if [ ! -d $PROJECT_DIR/build ];then
    mkdir -p $PROJECT_DIR/build
fi

cd $PROJECT_DIR/build

if [ ! -f Makefile ];then
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="$PROJECT_DIR" ../
fi

CMD="nice -n 15"
CMD="$CMD make $*"
$CMD || exit $?

#CMD="nice -n 15"
#CMD="$CMD make par-all"
#$CMD || exit $?

cd $MY_PWD
