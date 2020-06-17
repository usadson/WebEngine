/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "hooks.hpp"

#include "ccompat.hpp"
#include "data/text/named_characters.hpp"

void
Hooks::ExecuteStartupHooks() {
	NamedCharacters::Setup();
}

void
Hooks::ExecuteShutdownHooks() {
	CCompat::CloseStandardIO();
}
