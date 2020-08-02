#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

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
		inline explicit AbstractSBEncoding(const std::vector<Unicode::CodePoint> &indices) : Indices(indices) {
		}

		inline ~AbstractSBEncoding() override {
		}

		[[nodiscard]] bool
		Decode(const char *data, std::size_t size) override;
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

} // namespace TextEncoding
