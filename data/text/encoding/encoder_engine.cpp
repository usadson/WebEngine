/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "encoder_engine.hpp"

#include <algorithm>
#include <map>
#include <memory>

#include <cctype>

#include "logger.hpp"
#include "single_byte_encoding.hpp"
#include "utf8.hpp"

namespace Encoders {
	std::shared_ptr<TextEncoding::Encoding> UTF8 = std::make_shared<TextEncoding::UTF8>();
	std::shared_ptr<TextEncoding::Encoding> IBM866 = std::make_shared<TextEncoding::IBM866>();
	std::shared_ptr<TextEncoding::Encoding> ISO8859_2 = std::make_shared<TextEncoding::ISO8859_2>();

	std::shared_ptr<TextEncoding::Encoding> ISO8859_3 = std::make_shared<TextEncoding::ISO8859_3>();
	std::shared_ptr<TextEncoding::Encoding> ISO8859_4 = std::make_shared<TextEncoding::ISO8859_4>();
	std::shared_ptr<TextEncoding::Encoding> ISO8859_5 = std::make_shared<TextEncoding::ISO8859_5>();
	std::shared_ptr<TextEncoding::Encoding> ISO8859_6 = std::make_shared<TextEncoding::ISO8859_6>();
	std::shared_ptr<TextEncoding::Encoding> ISO8859_7 = std::make_shared<TextEncoding::ISO8859_7>();
	std::shared_ptr<TextEncoding::Encoding> ISO8859_8 = std::make_shared<TextEncoding::ISO8859_8>();
	std::shared_ptr<TextEncoding::Encoding> ISO8859_10 = std::make_shared<TextEncoding::ISO8859_10>();
	std::shared_ptr<TextEncoding::Encoding> ISO8859_13 = std::make_shared<TextEncoding::ISO8859_13>();
	std::shared_ptr<TextEncoding::Encoding> ISO8859_14 = std::make_shared<TextEncoding::ISO8859_14>();
	std::shared_ptr<TextEncoding::Encoding> ISO8859_15 = std::make_shared<TextEncoding::ISO8859_15>();
	std::shared_ptr<TextEncoding::Encoding> ISO8859_16 = std::make_shared<TextEncoding::ISO8859_16>();
	std::shared_ptr<TextEncoding::Encoding> KOI8_R = std::make_shared<TextEncoding::KOI8_R>();
	std::shared_ptr<TextEncoding::Encoding> KOI8_U = std::make_shared<TextEncoding::KOI8_U>();
	std::shared_ptr<TextEncoding::Encoding> Macintosh = std::make_shared<TextEncoding::Macintosh>();
	std::shared_ptr<TextEncoding::Encoding> Windows874 = std::make_shared<TextEncoding::Windows874>();
	std::shared_ptr<TextEncoding::Encoding> Windows1250 = std::make_shared<TextEncoding::Windows1250>();
	std::shared_ptr<TextEncoding::Encoding> Windows1251 = std::make_shared<TextEncoding::Windows1251>();
	std::shared_ptr<TextEncoding::Encoding> Windows1252 = std::make_shared<TextEncoding::Windows1252>();
	std::shared_ptr<TextEncoding::Encoding> Windows1253 = std::make_shared<TextEncoding::Windows1253>();
	std::shared_ptr<TextEncoding::Encoding> Windows1254 = std::make_shared<TextEncoding::Windows1254>();
	std::shared_ptr<TextEncoding::Encoding> Windows1255 = std::make_shared<TextEncoding::Windows1255>();
	std::shared_ptr<TextEncoding::Encoding> Windows1256 = std::make_shared<TextEncoding::Windows1256>();
	std::shared_ptr<TextEncoding::Encoding> Windows1257 = std::make_shared<TextEncoding::Windows1257>();
	std::shared_ptr<TextEncoding::Encoding> Windows1258 = std::make_shared<TextEncoding::Windows1258>();
	std::shared_ptr<TextEncoding::Encoding> XMacCyrillic = std::make_shared<TextEncoding::XMacCyrillic>();
} // namespace Encoders

std::map<std::string, std::shared_ptr<TextEncoding::Encoding> > Map = {
	{ "unicode-1-1-utf-8", Encoders::UTF8 },
	{ "utf-8", Encoders::UTF8 },
	{ "utf8", Encoders::UTF8 },

	{ "866", Encoders::IBM866 },
	{ "cp866", Encoders::IBM866 },
	{ "csibm866", Encoders::IBM866 },
	{ "ibm866", Encoders::IBM866 },

	{ "csisolatin2", Encoders::ISO8859_2 },
	{ "iso-8859-2", Encoders::ISO8859_2 },
	{ "iso-ir-101", Encoders::ISO8859_2 },
	{ "iso8859-2", Encoders::ISO8859_2 },
	{ "iso88592", Encoders::ISO8859_2 },
	{ "iso_8859-2", Encoders::ISO8859_2 },
	{ "iso_8859-2:1987", Encoders::ISO8859_2 },
	{ "l2", Encoders::ISO8859_2 },
	{ "latin2", Encoders::ISO8859_2 },

	{ "csisolatin3", Encoders::ISO8859_3 },
	{ "iso-8859-3", Encoders::ISO8859_3 },
	{ "iso-ir-109", Encoders::ISO8859_3 },
	{ "iso8859-3", Encoders::ISO8859_3 },
	{ "iso88593", Encoders::ISO8859_3 },
	{ "iso_8859-3", Encoders::ISO8859_3 },
	{ "iso_8859-3:1988", Encoders::ISO8859_3 },
	{ "l3", Encoders::ISO8859_3 },
	{ "latin3", Encoders::ISO8859_3 },

	{ "csisolatin4", Encoders::ISO8859_4 },
	{ "iso-8859-4", Encoders::ISO8859_4 },
	{ "iso-ir-110", Encoders::ISO8859_4 },
	{ "iso8859-4", Encoders::ISO8859_4 },
	{ "iso88594", Encoders::ISO8859_4 },
	{ "iso_8859-4", Encoders::ISO8859_4 },
	{ "iso_8859-4:1988", Encoders::ISO8859_4 },
	{ "l4", Encoders::ISO8859_4 },
	{ "latin4", Encoders::ISO8859_4 },

	{ "csisolatincyrillic", Encoders::ISO8859_5 },
	{ "cyrillic", Encoders::ISO8859_5 },
	{ "iso-8859-5", Encoders::ISO8859_5 },
	{ "iso-ir-144", Encoders::ISO8859_5 },
	{ "iso8859-5", Encoders::ISO8859_5 },
	{ "iso88595", Encoders::ISO8859_5 },
	{ "iso_8859-5", Encoders::ISO8859_5 },
	{ "iso_8859-5:1988", Encoders::ISO8859_5 },

	{ "arabic", Encoders::ISO8859_6 },
	{ "asmo-708", Encoders::ISO8859_6 },
	{ "csiso88596e", Encoders::ISO8859_6 },
	{ "csiso88596i", Encoders::ISO8859_6 },
	{ "csisolatinarabic", Encoders::ISO8859_6 },
	{ "ecma-114", Encoders::ISO8859_6 },
	{ "iso-8859-6", Encoders::ISO8859_6 },
	{ "iso-8859-6-e", Encoders::ISO8859_6 },
	{ "iso-8859-6-i", Encoders::ISO8859_6 },
	{ "iso-ir-127", Encoders::ISO8859_6 },
	{ "iso8859-6", Encoders::ISO8859_6 },
	{ "iso88596", Encoders::ISO8859_6 },
	{ "iso_8859-6", Encoders::ISO8859_6 },
	{ "iso_8859-6:1987", Encoders::ISO8859_6 },

	{ "csisolatingreek", Encoders::ISO8859_7 },
	{ "ecma-118", Encoders::ISO8859_7 },
	{ "elot_928", Encoders::ISO8859_7 },
	{ "greek", Encoders::ISO8859_7 },
	{ "greek8", Encoders::ISO8859_7 },
	{ "iso-8859-7", Encoders::ISO8859_7 },
	{ "iso-ir-126", Encoders::ISO8859_7 },
	{ "iso8859-7", Encoders::ISO8859_7 },
	{ "iso88597", Encoders::ISO8859_7 },
	{ "iso_8859-7", Encoders::ISO8859_7 },
	{ "iso_8859-7:1987", Encoders::ISO8859_7 },
	{ "sun_eu_greek", Encoders::ISO8859_7 },

	{ "csiso88598e", Encoders::ISO8859_8 },
	{ "csisolatinhebrew", Encoders::ISO8859_8 },
	{ "hebrew", Encoders::ISO8859_8 },
	{ "iso-8859-8", Encoders::ISO8859_8 },
	{ "iso-8859-8-e", Encoders::ISO8859_8 },
	{ "iso-ir-138", Encoders::ISO8859_8 },
	{ "iso8859-8", Encoders::ISO8859_8 },
	{ "iso88598", Encoders::ISO8859_8 },
	{ "iso_8859-8", Encoders::ISO8859_8 },
	{ "iso_8859-8:1988", Encoders::ISO8859_8 },
	{ "visual", Encoders::ISO8859_8 },

	/*	{ ISO-8859-8-I 	"csiso88598i"
		{ "iso-8859-8-i"
		{ "logical"*/
	{ "csisolatin6", Encoders::ISO8859_10 },
	{ "iso-8859-10", Encoders::ISO8859_10 },
	{ "iso-ir-157", Encoders::ISO8859_10 },
	{ "iso8859-10", Encoders::ISO8859_10 },
	{ "iso885910", Encoders::ISO8859_10 },
	{ "l6", Encoders::ISO8859_10 },
	{ "latin6", Encoders::ISO8859_10 },

	{ "iso-8859-13", Encoders::ISO8859_13 },
	{ "iso8859-13", Encoders::ISO8859_13 },
	{ "iso885913", Encoders::ISO8859_13 },

	{ "iso-8859-14", Encoders::ISO8859_14 },
	{ "iso8859-14", Encoders::ISO8859_14 },
	{ "iso885914", Encoders::ISO8859_14 },

	{ "csisolatin9", Encoders::ISO8859_15 },
	{ "iso-8859-15", Encoders::ISO8859_15 },
	{ "iso8859-15", Encoders::ISO8859_15 },
	{ "iso885915", Encoders::ISO8859_15 },
	{ "iso_8859-15", Encoders::ISO8859_15 },
	{ "l9", Encoders::ISO8859_15 },

	{ "iso-8859-16", Encoders::ISO8859_16 },

	{ "cskoi8r", Encoders::KOI8_R },
	{ "koi", Encoders::KOI8_R },
	{ "koi8", Encoders::KOI8_R },
	{ "koi8-r", Encoders::KOI8_R },
	{ "koi8_r", Encoders::KOI8_R },

	{ "koi8-ru", Encoders::KOI8_U },
	{ "koi8-u", Encoders::KOI8_U },

	{ "csmacintosh", Encoders::Macintosh },
	{ "mac", Encoders::Macintosh },
	{ "macintosh", Encoders::Macintosh },
	{ "x-mac-roman", Encoders::Macintosh },

	{ "dos-874", Encoders::Windows874 },
	{ "iso-8859-11", Encoders::Windows874 },
	{ "iso8859-11", Encoders::Windows874 },
	{ "iso885911", Encoders::Windows874 },
	{ "tis-620", Encoders::Windows874 },
	{ "windows-874", Encoders::Windows874 },

	{ "cp1250", Encoders::Windows1250 },
	{ "windows-1250", Encoders::Windows1250 },
	{ "x-cp1250", Encoders::Windows1250 },

	{ "cp1251", Encoders::Windows1251 },
	{ "windows-1251", Encoders::Windows1251 },
	{ "x-cp1251", Encoders::Windows1251 },

	{ "ansi_x3.4-1968", Encoders::Windows1252 },
	{ "ascii", Encoders::Windows1252 },
	{ "cp1252", Encoders::Windows1252 },
	{ "cp819", Encoders::Windows1252 },
	{ "csisolatin1", Encoders::Windows1252 },
	{ "ibm819", Encoders::Windows1252 },
	{ "iso-8859-1", Encoders::Windows1252 },
	{ "iso-ir-100", Encoders::Windows1252 },
	{ "iso8859-1", Encoders::Windows1252 },
	{ "iso88591", Encoders::Windows1252 },
	{ "iso_8859-1", Encoders::Windows1252 },
	{ "iso_8859-1:1987", Encoders::Windows1252 },
	{ "l1", Encoders::Windows1252 },
	{ "latin1", Encoders::Windows1252 },
	{ "us-ascii", Encoders::Windows1252 },
	{ "windows-1252", Encoders::Windows1252 },
	{ "x-cp1252", Encoders::Windows1252 },

	{ "cp1253", Encoders::Windows1253 },
	{ "windows-1253", Encoders::Windows1253 },
	{ "x-cp1253", Encoders::Windows1253 },

	{ "cp1254", Encoders::Windows1254 },
	{ "csisolatin5", Encoders::Windows1254 },
	{ "iso-8859-9", Encoders::Windows1254 },
	{ "iso-ir-148", Encoders::Windows1254 },
	{ "iso8859-9", Encoders::Windows1254 },
	{ "iso88599", Encoders::Windows1254 },
	{ "iso_8859-9", Encoders::Windows1254 },
	{ "iso_8859-9:1989", Encoders::Windows1254 },
	{ "l5", Encoders::Windows1254 },
	{ "latin5", Encoders::Windows1254 },
	{ "windows-1254", Encoders::Windows1254 },
	{ "x-cp1254", Encoders::Windows1254 },

	{ "cp1255", Encoders::Windows1255 },
	{ "windows-1255", Encoders::Windows1255 },
	{ "x-cp1255", Encoders::Windows1255 },

	{ "cp1256", Encoders::Windows1256 },
	{ "windows-1256", Encoders::Windows1256 },
	{ "x-cp1256", Encoders::Windows1256 },

	{ "cp1257", Encoders::Windows1257 },
	{ "windows-1257", Encoders::Windows1257 },
	{ "x-cp1257", Encoders::Windows1257 },

	{ "cp1258", Encoders::Windows1258 },
	{ "windows-1258", Encoders::Windows1258 },
	{ "x-cp1258", Encoders::Windows1258 },

	{ "x-mac-cyrillic", Encoders::XMacCyrillic },
	{ "x-mac-ukrainian", Encoders::XMacCyrillic },
};

std::vector<Unicode::CodePoint>
TextEncoding::EncoderEngine::DecodeData(const char *data, std::size_t size, std::string encoding) {
	std::transform(encoding.begin(), encoding.end(), encoding.begin(), [](unsigned char c) { return std::tolower(c); });

	auto it = Map.find(encoding);
	if (it == Map.end()) {
		Logger::Warning("TextEncoding::EncoderEngine::DecodeData", "Not found: " + encoding);
		return {};
	}

	// TODO should we reset and/or check for concurrency and shit?
	if (!it->second->Decode(data, size)) {
		Logger::Warning("TextEncoding::EncoderEngine::DecodeData", "Failed to decode: " + encoding);
		return {};
	}

	return it->second->Output;
}
