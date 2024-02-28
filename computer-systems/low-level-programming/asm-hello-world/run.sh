#!/bin/zsh
nasm -fmacho64 hello.asm && \
  ld hello.o -o hello -macos_version_min 11.0 -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem -no_pie && \
  ./hello
