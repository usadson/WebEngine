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

#include "data/text/unicode.hpp"
#include "encoding.hpp"

namespace TextEncoding {

	/**
	 * The Abstract SingleByte Encodings 
	 */
	class AbstractSBEncoding : public Encoding {
	private: // Private Properties
		const std::vector<Unicode::CodePoint> &Indices;
	public:
		inline explicit
		AbstractSBEncoding(const std::vector<Unicode::CodePoint> &indices)
			: Indices(indices) {}

		inline virtual
		~AbstractSBEncoding() override {}

		bool
		Decode(const char *data, size_t size) override;
	};

	/* https://encoding.spec.whatwg.org/index-ibm866.txt */
	struct IBM866 : public AbstractSBEncoding {
		IBM866();
	};

	struct ISO8859_2 : public AbstractSBEncoding {
		ISO8859_2();
	};

	struct ISO8859_3 : public AbstractSBEncoding {
		ISO8859_3();
	};

	struct ISO8859_4 : public AbstractSBEncoding {
		ISO8859_4();
	};

	struct ISO8859_5 : public AbstractSBEncoding {
		ISO8859_5();
	};

	struct ISO8859_6 : public AbstractSBEncoding {
		ISO8859_6();
	};

	struct ISO8859_7 : public AbstractSBEncoding {
		ISO8859_7();
	};

	struct ISO8859_8 : public AbstractSBEncoding {
		ISO8859_8();
	};

	struct ISO8859_10 : public AbstractSBEncoding {
		ISO8859_10();
	};

	struct ISO8859_13 : public AbstractSBEncoding {
		ISO8859_13();
	};

	struct ISO8859_14 : public AbstractSBEncoding {
		ISO8859_14();
	};

	struct ISO8859_15 : public AbstractSBEncoding {
		ISO8859_15();
	};

	struct ISO8859_16 : public AbstractSBEncoding {
		ISO8859_16();
	};

	struct KOI8_R : public AbstractSBEncoding {
		KOI8_R();
	};

	struct KOI8_U : public AbstractSBEncoding {
		KOI8_U();
	};

	struct Macintosh : public AbstractSBEncoding {
		Macintosh();
	};

	struct Windows874 : public AbstractSBEncoding {
		Windows874();
	};

	struct Windows1250 : public AbstractSBEncoding {
		Windows1250();
	};

	struct Windows1251 : public AbstractSBEncoding {
		Windows1251();
	};

	struct Windows1252 : public AbstractSBEncoding {
		Windows1252();
	};

	struct Windows1253 : public AbstractSBEncoding {
		Windows1253();
	};

	struct Windows1254 : public AbstractSBEncoding {
		Windows1254();
	};

	struct Windows1255 : public AbstractSBEncoding {
		Windows1255();
	};

	struct Windows1256 : public AbstractSBEncoding {
		Windows1256();
	};

	struct Windows1257 : public AbstractSBEncoding {
		Windows1257();
	};
	
	struct Windows1258 : public AbstractSBEncoding {
		Windows1258();
	};

	struct XMacCyrillic : public AbstractSBEncoding {
		XMacCyrillic();
	};
}
