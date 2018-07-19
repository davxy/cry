#!/bin/sh

# Replace tabs with 4 spaces
find $1 -not -path '*/\.*' -type f | grep -E ".(h|c)$" | xargs -L1 ./stylecheck.sh

