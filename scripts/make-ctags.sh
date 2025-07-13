#!/bin/bash
TAGS=$WEBROOT/tags
rm $TAGS 2>/dev/null
cd $WEBROOT
find -type f \
  -path '*src/*.*pp' \
  > $WEBROOT/cscope.files
ctags -R -f $TAGS  --links=no \
	--exclude=/usr/include \
	--totals=no \
	--tag-relative=yes \
	--if0=no \
	--fields=+a+f+i+K+n+s+S+z+t \
	--C++-kinds=+f+c+e-g-l+m-u+v \
	-h .cpp.hpp &
cscope -R -b -i $WEBROOT/cscope.files &
