#!/bin/sh


for file in $ACTION_SOURCE; do
    gcc $ACTION_BUILD "$ACTION_OBJ${file%.*}".o "$file"
done
