/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
	};

	std::vector<Input> inputs = {
		{ "credits",	false },
		{ "test",		true },
	};

	std::vector<Output> outputs;
	std::vector<std::string> textOutput;

}

namespace Options {

	std::map<Type, std::string> values = {
		{ Type::TLS_SECURITY_LEVEL, "secure" },
	};

}

std::optional<const std::optional<std::string> *>
Options::GetCommandLineParameter(std::string name) {
	auto result = std::find_if(std::begin(CommandLineParser::outputs),
		std::end(CommandLineParser::outputs),
		[name](const auto &entry) -> bool {
			return strcasecmp(name.c_str(), entry.name.c_str()) == 0;
		}
	);

	if (result != std::end(CommandLineParser::outputs))
		return { &result->value };
	else
		return {};
}

bool
Options::ParseCommandLine(int argc, const char **argv) {
	if (argc == 0 || argc == 1)
		return true;

	size_t i;
	std::optional<std::string_view> prevName;

	for (i = 1; i < (size_t) argc; i++) {
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
				std::string name(std::begin(strview) + 2);

				auto result = std::find_if(std::begin(CommandLineParser::inputs),
										   std::end(CommandLineParser::inputs),
					[name](const auto &entry) -> bool {
						return strcasecmp(name.c_str(), entry.name.c_str()) == 0;
					}
				);

				if (result != std::end(CommandLineParser::inputs) && result->optionHasValue) {
					if (i == (size_t) argc - 1) {
						std::cerr << "Invalid command line option: " << result->name
								  << " (required option value)" << std::endl;
						return false;
					}
					prevName = strview;
				} else {
					CommandLineParser::outputs.push_back({ 
						std::string(std::begin(strview) + 2, std::end(strview)),
						{}
					});
				}
			} else {
				CommandLineParser::outputs.push_back({ 
					std::string(std::begin(strview) + 2, equals - 2),
					std::string(std::begin(strview) + equals + 1)
				});
			}
		} else if (prevName.has_value()) {
			CommandLineParser::outputs.push_back({ 
				std::string(std::begin(prevName.value()) + 2, std::end(prevName.value())),
				std::string(strview)
			});
			prevName.reset();
		} else {
			CommandLineParser::textOutput.push_back(std::string(strview));
		}
	}

	return true;
}
