#!/bin/bash 

mkdir -p /usr/local/duoyi/
cp -r img /usr/local/duoyi/
cp duoyi preferences /usr/local/duoyi/
cp .duoyirc /usr/local/duoyi/
cp readme /usr/local/duoyi/
cp LICENSE /usr/local/duoyi/

ln -s /usr/local/duoyi/duoyi /bin/duoyi

cp duoyi.desktop /home/brisk/.local/share/applications/ 

exit 0
