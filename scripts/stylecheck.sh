#!/bin/bash

dirs=$1

if [[ $dirs == "" ]]
then
    dirs="../include ../src ../test"
fi
echo "Checking $dirs"

# Replace tabs with 4 spaces
find $dirs -not -path '*/\.*' -type f | grep -E ".(h|c)$" | xargs -L1 uncrustify -c uncrustify.cfg --no-backup
