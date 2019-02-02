#!/bin/bash

base=$(cd $1; pwd)
target=$(cd $2; pwd)"/tsc"


if [[ $1 == "" ]]; then
    echo "usage: $0 crydir <tscdir>"
    exit
fi
if [[ $2 == "" ]]; then
    target=$(pwd)"/tsc"
fi

echo "Building TSC from CRY directory"
echo "Src: $base"
echo "Dst: $target"

skip="head.tmp maketsc.sh build"

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

rm -rf $target
mkdir $target

files=`find $base -type f | grep -v '/\.'`

for src in $files
do
    fname=$(echo $src | sed 's|^'$base'\/||')
    dir=`dirname $fname`
    mkdir -p $target/$dir
    dst=$target/$(echo $fname | sed 's/cry_/tsc_/' | sed 's/cry\./tsc\./')
    echo $dst
    cp $src $dst
    sed -i -e 's/cry_/tsc_/g' -e 's/CRY_/TSC_/g' $dst
    sed -i -e 's/<cry/<tsc/g' -e 's/\"cry/\"tsc/g' $dst
    ext="${fname##*.}"
    bname=`basename $dst`
    if [ "$ext" = "h" ] || [ "$ext" = "c" ]
    then
        cat head.tmp > tmp
        cat $dst >> tmp
        mv tmp $dst
    fi
done

# Replaces library name
set -x
sed -i 's/cry/tsc/g' $target/Makefile $target/test/Makefile
set +x

# Rename the include tree dir
mv $target/include/cry $target/include/tsc

# Remove not required files
rm head.tmp
cd $target
rm -rf $skip
cd -
