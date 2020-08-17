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

# Create the full thing AFAP.
make -j$threads all

# If the previous failed, run the following to ensure everything is compiled
# correctly:
make
