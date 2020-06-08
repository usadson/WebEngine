/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
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
	static const std::array<CommandLineParser::Input, 3> inputs = { {
		{ "copyright",	false },
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
