#!/usr/bin/env sh
sed -i "s#https\?://mirror.msys2.org/#https://mirrors.tuna.tsinghua.edu.cn/msys2/#g" /etc/pacman.d/mirrorlist*; 
pacman -Sy mingw-w64-x86_64-openssl mingw-w64-x86_64-python mingw-w64-x86_64-libffi mingw-w64-x86_64-python-pip;