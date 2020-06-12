#!/bin/bash
#
# Copyright (C) 2020 Tristan. All Rights Reserved.
# This file is licensed under the BSD 2-Clause license.
# See the COPYING file for licensing information.

# grep -c ^processor /proc/cpuinfo will count the amount of cores on the CPU,
# and awk will multiply those with 1.5 to improve speed. If we use this value
# on the -j option of make, it will run the build much faster.
threads=$(grep -c ^processor /proc/cpuinfo | awk '{print $1 * 1.5}')

# Create the appropriate output folders:
make bin/test.txt

# Only build the objects concurrently, not the final executable binary, since
# other objects may not have been built yet when the executable is being put
# together.
make -j$threads objects

# Make the binary executable:
make
