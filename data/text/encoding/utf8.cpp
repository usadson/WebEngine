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

#include "utf8.hpp"

#include <iostream>

#include <cstdio>

#include "logger.hpp"

namespace TextEncoding {
	std::vector<Unicode::CodePoint>
	UTF8::ASCIIDecode(const char *data, size_t size) {
		std::vector<Unicode::CodePoint> result(size);
		size_t i;

		for (i = 0; i < size; i++)
			result.push_back((Unicode::CodePoint) data[i]);

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
				} else if (currentByte >= 0xE0 && currentByte <= 0xEF) {
					if (currentByte == 0xE0)
						LowerBoundary = 0xA0;
					else if (currentByte == 0xED)
						UpperBoundary = 0x9F;
					BytesNeeded = 2;
					CodePoint = currentByte & 0xF;
				} else if (currentByte >= 0xF0 && currentByte <= 0xF4) {
					if (currentByte == 0xF0)
						LowerBoundary = 0x90;
					else if (currentByte == 0xF4)
						UpperBoundary = 0x8F;
					BytesNeeded = 3;
					CodePoint = currentByte & 0x7;
				} else {
					Logger::Error("TextEncoding::UTF8::Decode", "Character out of scope.");
					return false;
				}

				continue;
			}

			// 'in range ... to ... `inclusive`, then'??
			if (currentByte < LowerBoundary || currentByte > UpperBoundary) {
				CodePoint = 0;
				BytesNeeded = 0;
				BytesSeen = 0;
				LowerBoundary = 0x80;
				UpperBoundary = 0xBF;
				// TODO 'Prepend byte to stream'
				Logger::Error("TextEncoding::UTF8::Decode", "Character out of boundaries.");
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
