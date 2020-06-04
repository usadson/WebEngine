/**
 * Copyright (C) 2020 Tristan
 *
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "options.hpp"

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <cstring>

namespace CommandLineParser {

	struct Input {
		std::string name;
		bool optionHasValue;
	};

	struct Output {
		std::string name;
		std::optional<std::string> value;

		inline
		Output(std::string name, std::optional<std::string> value)
			: name(std::move(name)), value(std::move(value)) {
		}
	};

	std::vector<Output> outputs;
	std::vector<std::string> textOutput;

}

std::optional<const std::optional<std::string> *>
Options::GetCommandLineParameter(const std::string &name) {
	auto result = std::find_if(std::begin(CommandLineParser::outputs),
		std::end(CommandLineParser::outputs),
		[name](const auto &entry) -> bool {
			return strcasecmp(name.c_str(), entry.name.c_str()) == 0;
		}
	);

	if (result != std::end(CommandLineParser::outputs)) {
		return { &result->value };
	}

	return {};
}

bool
ParseEqualsOption(const std::string_view &strview,
				  std::optional<std::string_view> *prevName,
				  bool isLastOption) {
	static const std::array<CommandLineParser::Input, 2> inputs = { {
		{ "credits",	false },
		{ "test",		true },
	} };

	std::string name(std::begin(strview) + 2);

	const auto *result = std::find_if(std::begin(inputs), std::end(inputs),
		[name](const auto &entry) -> bool {
			return strcasecmp(name.c_str(), entry.name.c_str()) == 0;
		}
	);

	if (result != std::end(inputs) && result->optionHasValue) {
		if (isLastOption) {
			std::cerr << "Invalid command line option: " << result->name
						<< " (required option value)" << std::endl;
			return false;
		}
		*prevName = strview;
	} else {
		CommandLineParser::outputs.emplace_back(
			std::string(std::begin(strview) + 2, std::end(strview)),
			std::string()
		);
	}

	return true;
}


bool
Options::ParseCommandLine(int argc, const char **argv) {
	if (argc == 0 || argc == 1) {
		return true;
	}

	std::optional<std::string_view> prevName;

	for (size_t i = 1; i < static_cast<size_t>(argc); i++) {
		std::string_view strview(argv[i]);

		if (argv[i][0] == '\0')
			continue;

		if (strview[0] == '-' && strview[1] == '-') {
			auto equals = strview.find('=');

			if (prevName.has_value()) {
				std::cerr << "Invalid command line option: " << prevName.value()
						  << " (required option value)" << std::endl;
				return false;
			}

			/* Option name is empty */
			if (strview.length() == 2 || equals == 2) {
				std::cerr << "Invalid command line option: " << strview
						  << " (option name is empty)" << std::endl;
				return false;
			}

			if (equals == std::string::npos) {
				if (!ParseEqualsOption(strview, &prevName, i == static_cast<size_t>(argc) - 1))
					return false;
			} else {
				CommandLineParser::outputs.emplace_back(
					std::string(std::begin(strview) + 2, equals - 2),
					std::string(std::begin(strview) + equals + 1)
				);
			}
		} else if (prevName.has_value()) {
			CommandLineParser::outputs.emplace_back(
				std::string(std::begin(prevName.value()) + 2, std::end(prevName.value())),
				std::string(strview)
			);
			prevName.reset();
		} else {
			CommandLineParser::textOutput.emplace_back(strview);
		}
	}

	return true;
}
