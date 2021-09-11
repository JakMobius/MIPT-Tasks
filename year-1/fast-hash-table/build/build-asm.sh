#!/bin/sh

for file in $ACTION_SOURCE; do
    nasm $ACTION_BUILD $ACTION_OBJ"${file%.*}".o "$file"
done
