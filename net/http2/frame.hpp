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

#pragma once

#include <vector>

#include <cstdint>

namespace Net {
	namespace HTTP {
		namespace H2 {

			namespace FrameFlags {
				const uint8_t ACK = 0x1;
			}

			enum FrameType {
				DATA = 0x0,
				HEADERS = 0x1,
				PRIORITY = 0x2,
				RST_STREAM = 0x3,
				SETTINGS = 0x4,
				PUSH_PROMISE = 0x5,
				PING = 0x6,
				GOAWAY = 0x7,
				WINDOW_UPDATE = 0x8,
				CONTINUATION = 0x9,
				ALTSVC = 0xA,
				ORIGIN = 0xB
			};

			struct Frame {
				uint32_t length : 24;
				uint8_t type;
				uint8_t flags;
				uint32_t stream;
				std::vector<char> payload;
			};

		}
	}
}
