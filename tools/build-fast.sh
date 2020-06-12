#!/usr/bin/bash
#
# Copyright (C) 2020 Tristan. All Rights Reserved.
# This file is licensed under the BSD 2-Clause license.
# See the COPYING file for licensing information.

make bin/test.txt
make -j`grep -c ^processor /proc/cpuinfo | awk '{print $1 * 1.5}'` objects
make
