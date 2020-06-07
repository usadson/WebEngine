/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "utf8.hpp"

#include <iostream>
#include <sstream>

#include "logger.hpp"

namespace TextEncoding {
	std::vector<Unicode::CodePoint>
	UTF8::ASCIIDecode(const char *data, size_t size) {
		std::vector<Unicode::CodePoint> result(size);

		for (size_t i = 0; i < size; i++)
			result.push_back(static_cast<Unicode::CodePoint>(data[i]));

		return result;
	}

	bool
	UTF8::Decode(const char *data, size_t size) {
		size_t bytePosition;

		bytePosition = 0;

		Output.clear();
		CodePoint = 0;
		BytesSeen = 0;
		BytesNeeded = 0;
		LowerBoundary = 0x80;
		UpperBoundary = 0xBF;

		while (true) {
			if (bytePosition == size) {
				if (BytesNeeded != 0) {
					BytesNeeded = 0;
					Logger::Warning("TextEncoding::UTF8::Decode", "BytesLeft = 0 but BytesNeeded > 0");
					return false;
				}

				// finished:
				return true;
			}

			uint8_t currentByte = data[bytePosition++];

			if (BytesNeeded == 0) {
				// TODO Is the ""to"" of (`0xE0 ""to"" 0xEF`) inclusive or not?

				if (currentByte <= 0x7F) {
					Output.push_back(currentByte);
					continue;
				}

				if (currentByte >= 0xC2 && currentByte <= 0xDF) {
					BytesNeeded = 1;
					CodePoint = currentByte & 0x1F;
					continue;
				}

				if (currentByte >= 0xE0 && currentByte <= 0xEF) {
					if (currentByte == 0xE0)
						LowerBoundary = 0xA0;
					else if (currentByte == 0xED)
						UpperBoundary = 0x9F;
					BytesNeeded = 2;
					CodePoint = currentByte & 0xF;
					continue;
				}

				if (currentByte >= 0xF0 && currentByte <= 0xF4) {
					if (currentByte == 0xF0)
						LowerBoundary = 0x90;
					else if (currentByte == 0xF4)
						UpperBoundary = 0x8F;
					BytesNeeded = 3;
					CodePoint = currentByte & 0x7;
					continue;
				}

				std::stringstream info;
				info << "Octet out of scope: 0x"
						<< std::hex << static_cast<uint16_t>(currentByte)
						<< std::dec;
				Logger::Error("TextEncoding::UTF8::Decode", info.str());
				return false;
			}

			// 'in range ... to ... `inclusive`, then'??
			if (currentByte < LowerBoundary || currentByte > UpperBoundary) {
				CodePoint = 0;
				BytesNeeded = 0;
				BytesSeen = 0;
				LowerBoundary = 0x80;
				UpperBoundary = 0xBF;
				// TODO 'Prepend byte to stream'
				std::stringstream info;
				info << "Character out of boundaries: [0x" << std::hex
					 << static_cast<uint16_t>(LowerBoundary) << ", 0x"
					 << static_cast<uint16_t>(UpperBoundary) << "], value: 0x"
					 << static_cast<uint16_t>(currentByte) << std::dec;
				Logger::Error("TextEncoding::UTF8::Decode", info.str());
				return false;
			}

			LowerBoundary = 0x80;
			UpperBoundary = 0xBF;
			CodePoint = (CodePoint << 6) | (currentByte & 0x3F);
			BytesSeen += 1;
			if (BytesSeen != BytesNeeded)
				continue;
			Output.push_back(CodePoint);
			CodePoint = 0;
			BytesNeeded = 0;
			BytesSeen = 0;
		}
	}
}
