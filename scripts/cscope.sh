#!/bin/bash

cd $WEBROOT
find mods -type f -name '*.hpp' | >> ./cscope.files 
find mods -type f -name '*.cpp' | >> ./cscope.files 
ls *.h *.cpp *.hpp >> ./cscope.files 
cscope -Rb

echo "cscope.out and cscope.files are in $WEBROOT"
