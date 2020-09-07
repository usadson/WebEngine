#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

class Tab;

#include "rendering/view.hpp"

class Tab {
public:
	Rendering::View view{this};
};
