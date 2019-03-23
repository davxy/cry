#!/bin/bash

# Prepare source and target folders

if [[ $1 == "" ]]; then
    base=".."
else
    base=$1
fi
base=$(cd $base; pwd)

if [[ $2 == "" ]]; then
    target="."
else
    target=$2
fi
target=$(cd $target; pwd)/TSC

echo "Building TSC from CRY directory"
echo "Src: $base"
echo "Dst: $target"
# Target cleanup
rm -rf $target
mkdir $target

#
# Files and folders to skip
#
skip="head.tmp maketsc.sh build"

# Copyright header
cat << EOF > head.tmp
/*
 * Copyright (C) 2012-2019, Terranova Software. All rights reserved.
 *
 * This file is part of TSC Library.
 *
 * TSC can not be copied and/or distributed without the express permission
 * of Terranova Software.
 */

EOF

# Find files
files=`find $base -type f | grep -Ev '(.git|ide|doc|build|scripts|.o$)'`

# Process files
for src in $files
do
    # Create the destination file (and dir)
    fname=$(echo $src | sed 's|^'$base'\/||')
    dir=`dirname $fname`
    mkdir -p $target/$dir
    dst=$target/$fname
    cp $src $dst
    # Replace names within the file
    sed -i -e 's/cry_/tsc_/g' -e 's/CRY_/TSC_/g' $dst
    sed -i -e 's/<cry/<tsc/g' -e 's/\"cry/\"tsc/g' $dst
    # Prepend TSC header to sources
    ext="${fname##*.}"
    bname=`basename $dst`
    if [ "$ext" = "h" ] || [ "$ext" = "c" ]
    then
        cat head.tmp > tmp
        cat $dst >> tmp
        mv tmp $dst
    fi
done

# Replaces library name in makefiles
sed -i 's/cry/tsc/g' $target/Makefile $target/test/Makefile

# Rename the include tree dir
mv $target/include/cry $target/include/tsc

# Remove not required files
rm head.tmp
cd $target
rm -rf $skip
cd -
