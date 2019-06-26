#!/bin/bash

multibyte_check()
{
    file=$1
    if [[ ! -z $(file $file | grep UTF-8) ]]
    then
        echo "> Found multibyte character"
        grep --color='auto' -P -n "[^\x00-\x7F]" $file
    fi
}

dirs=$1

if [[ $dirs == "" ]]
then
    dirs="../include ../src ../test"
fi
echo "Checking $dirs"

files=$(find $dirs -type f | grep -E ".(h|c)$")
for file in $files
do
    uncrustify -c uncrustify.cfg --no-backup $file
    #clang-format -style=file -i -verbose
    multibyte_check $file
done


