#!/usr/bin/bash
make bin/test.txt
make -j`grep -c ^processor /proc/cpuinfo | awk '{print $1 * 1.5}'` objects
make
