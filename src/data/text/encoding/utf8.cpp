#include "utf8.hpp"

#include <iostream>

#include <cstdio>

#include "logger.hpp"

namespace TextEncoding {
	std::vector<Unicode::CodePoint> UTF8::ASCIIDecode(const char *data, size_t size) {
		std::vector<Unicode::CodePoint> result(size);
		size_t i;

		for (i = 0; i < size; i++)
			result.push_back((Unicode::CodePoint) data[i]);

		return result;
	}

	bool UTF8::Decode(const char *data, size_t size) {
		uint8_t currentByte;
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

			currentByte = data[bytePosition++];

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
