#!/bin/sh

# Be sure we are using unix file format
dos2unix $1

# Replace tabs with 4 spaces
sed -i "s/\t/    /g" $1

# Remove multiple empty lines at end of file

n=`awk 'BEGIN { tot=0; nl=0; }
        { tot+=length($0)+1; if(NF==0) { nl++; } else { nl=0; } }
        END { printf("%d\n", tot-nl) }' $1`
truncate -s $n $1

