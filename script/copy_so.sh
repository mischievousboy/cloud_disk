#!/bin/bash
# shellcheck disable=SC2045
for dir in `ls $1`
  do
    cp $(ldd $1/$dir/* | awk '{if (match($3,"/")){ printf("%s "),$3 } }') $1/$dir
  done