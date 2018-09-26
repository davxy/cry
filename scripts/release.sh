#!/bin/bash



BUILD_DIR=
VERSION=
MACHINE=
FILEFMT=
COMPILER=
SYSTEM=

# Release name
RELEASE=""

usage()
{
    cat << EOF
    usage: $0 [options]

    OPTIONS
        -d build directory
           If empty will be set as the last modified dir under
           the "build" folder.
        -v version in dotted format
        -m machine type (x86,mips,arm,cortex-m3,...)
           If empty will be set equal to build directory
        -o object file format (elf,coff,...)
        -c compiler
        -h help
EOF
}

while getopts "d:v:m:o:c:s:h" option
do
    case "${option}"
    in
    d) BUILD_DIR=${OPTARG};;
    v) VERSION=${OPTARG};;
    m) MACHINE=${OPTARG};;
    o) FILEFMT=${OPTARG};;
    c) COMPILER=${OPTARG};;
    h) usage; exit 0;;
    esac
done

#
# Build directory
#
if [[ -z $BUILD_DIR ]]
then
    BUILD_DIR=build/$(ls -t build | head -1)
fi

#
# Library version
#
if [[ -z $VERSION ]]
then
    MAJOR=$(cat include/cry/version.h | grep "define CRY_MAJOR" | awk '{ print $3; }')
    MINOR=$(cat include/cry/version.h | grep "define CRY_MINOR" | awk '{ print $3; }')
    PATCH=$(cat include/cry/version.h | grep "define CRY_PATCH" | awk '{ print $3; }')
    VERSION=$MAJOR.$MINOR.$PATCH
fi
RELEASE=$VERSION

#
# Machine
#
if [[ -n $MACHINE ]]
then
    VERSION=$VERSION-$MACHINE
fi

#
# File format
#
if [[ -z $FILEFMT ]]
then
    FILEFMT=$(objdump -f "$BUILD_DIR/version.o" | grep "file format" | awk '{print $4; }')
fi
VERSION=$VERSION-$FILEFMT

#
# Compiler
#
if [[ -e $COMPILER ]]
then
    VERSION=$VERSION "-" $COMPILER
fi

RELEASE=$(date +"%y%m%d")
VERSION=$VERSION-$RELEASE

#
# Add files to the archive
#
LIBFILES="CHANGELOG.md"
if [ -f $BUILD_DIR/libcry.a ]
then
    cp $BUILD_DIR/libcry.a libcry.a
    LIBFILES="$LIBFILES libcry.a"
fi
if [ -f $BUILD_DIR/libcry.so ]
then
    cp $BUILD_DIR/libcry.so libcry.so
    LIBFILES="$LIBFILES libcry.so"
fi

#
# Create the archive
#
tar cvzf cry-$VERSION.tar.gz include/* $LIBFILES
