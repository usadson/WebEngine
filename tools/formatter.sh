#!/bin/bash
#
# Copyright (C) 2020 Tristan. All Rights Reserved.
# This file is licensed under the BSD 2-Clause license.
# See the COPYING file for licensing information.
find . -iname '*.hpp' -o -iname '*.cpp' -exec clang-format -i {} +
