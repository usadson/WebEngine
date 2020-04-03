#include "insert_initial.hpp"

#include <iostream>
#include <vector>

const std::vector<const char *> quirkyPublicIdentifiersMissingSystem = {
	"-//W3C//DTD HTML 4.01 Frameset//",
	"-//W3C//DTD HTML 4.01 Transitional//"
};

const std::vector<const char *> quirkyPublicIdentifiers = {
	"-//W3O//DTD W3 HTML Strict 3.0//EN//",
	"-/W3C/DTD HTML 4.0 Transitional/EN",
	"HTML",
	"+//Silmaril//dtd html Pro v0r11 19970101//",
	"-//AS//DTD HTML 3.0 asWedit + extensions//",
	"-//AdvaSoft Ltd//DTD HTML 3.0 asWedit + extensions//",
	"-//IETF//DTD HTML 2.0 Level 1//",
	"-//IETF//DTD HTML 2.0 Level 2//",
	"-//IETF//DTD HTML 2.0 Strict Level 1//",
	"-//IETF//DTD HTML 2.0 Strict Level 2//",
	"-//IETF//DTD HTML 2.0 Strict//",
	"-//IETF//DTD HTML 2.0//",
	"-//IETF//DTD HTML 2.1E//",
	"-//IETF//DTD HTML 3.0//",
	"-//IETF//DTD HTML 3.2 Final//",
	"-//IETF//DTD HTML 3.2//",
	"-//IETF//DTD HTML 3//",
	"-//IETF//DTD HTML Level 0//",
	"-//IETF//DTD HTML Level 1//",
	"-//IETF//DTD HTML Level 2//",
	"-//IETF//DTD HTML Level 3//",
	"-//IETF//DTD HTML Strict Level 0//",
	"-//IETF//DTD HTML Strict Level 1//",
	"-//IETF//DTD HTML Strict Level 2//",
	"-//IETF//DTD HTML Strict Level 3//",
	"-//IETF//DTD HTML Strict//",
	"-//IETF//DTD HTML//",
	"-//Metrius//DTD Metrius Presentational//",
	"-//Microsoft//DTD Internet Explorer 2.0 HTML Strict//",
	"-//Microsoft//DTD Internet Explorer 2.0 HTML//",
	"-//Microsoft//DTD Internet Explorer 2.0 Tables//",
	"-//Microsoft//DTD Internet Explorer 3.0 HTML Strict//",
	"-//Microsoft//DTD Internet Explorer 3.0 HTML//",
	"-//Microsoft//DTD Internet Explorer 3.0 Tables//",
	"-//Netscape Comm. Corp.//DTD HTML//",
	"-//Netscape Comm. Corp.//DTD Strict HTML//",
	"-//O'Reilly and Associates//DTD HTML 2.0//",
	"-//O'Reilly and Associates//DTD HTML Extended 1.0//",
	"-//O'Reilly and Associates//DTD HTML Extended Relaxed 1.0//",
	"-//SQ//DTD HTML 2.0 HoTMetaL + extensions//",
	"-//SoftQuad Software//DTD HoTMetaL PRO 6.0::19990601::extensions to HTML 4.0//",
	"-//SoftQuad//DTD HoTMetaL PRO 4.0::19971010::extensions to HTML 4.0//",
	"-//Spyglass//DTD HTML 2.0 Extended//",
	"-//Sun Microsystems Corp.//DTD HotJava HTML//",
	"-//Sun Microsystems Corp.//DTD HotJava Strict HTML//",
	"-//W3C//DTD HTML 3 1995-03-24//",
	"-//W3C//DTD HTML 3.2 Draft//",
	"-//W3C//DTD HTML 3.2 Final//",
	"-//W3C//DTD HTML 3.2//",
	"-//W3C//DTD HTML 3.2S Draft//",
	"-//W3C//DTD HTML 4.0 Frameset//",
	"-//W3C//DTD HTML 4.0 Transitional//",
	"-//W3C//DTD HTML Experimental 19960712//",
	"-//W3C//DTD HTML Experimental 970421//",
	"-//W3C//DTD W3 HTML//",
	"-//W3O//DTD W3 HTML 3.0//",
	"-//WebTechs//DTD Mozilla HTML 2.0//",
	"-//WebTechs//DTD Mozilla HTML//"
};

bool IsQuirkyDoctype(HTML::Tokenizer::DoctypeToken *token) {
	if (token->ForceQuirks)
		return true;

	if (!token->Name.has_value())
		return true;

	if (token->Name.value().EqualsA("html"))
		return true;

	if (token->SystemIdentifier.has_value()) {
		if (token->Name.value().EqualsA("html"))
		if (token->SystemIdentifier.value().EqualsA("http://www.ibm.com/data/dtd/v11/ibmxhtml1-transitional.dtd")) {
			return true;
		}
	} else if (token->PublicIdentifier.has_value()) {
		for (const auto &string : quirkyPublicIdentifiersMissingSystem) {
			if (token->PublicIdentifier.value().EqualsA(string)) {
				return true;
			}
		}
	}

	if (token->PublicIdentifier.has_value()) {
		for (const auto &string : quirkyPublicIdentifiers) {
			if (token->PublicIdentifier.value().EqualsA(string)) {
				return true;
			}
		}
	}

	return false;
}

void HTML::InsertionModes::Initial::EmitToken(HTML::Tokenizer::Token &inToken) {
	if (auto *startTagToken = dynamic_cast<HTML::Tokenizer::StartTagToken *>(&inToken)) {
		if (startTagToken->AttributeName.length() != 0)
			startTagToken->AddTokenAttribute(Context);

		std::cout << "++" << startTagToken->TagName << std::endl;

		if (startTagToken->Attributes.size() != 0) {
			std::cout << "  -> Attribute List (" << startTagToken->Attributes.size() << "):" << std::endl;
			for (const auto &attribute : startTagToken->Attributes) {
				std::cout << "    -> \"" << attribute.first << "\" = \"" << attribute.second << "\"" << std::endl;
			}
		}
	} else if (auto *endTagToken = dynamic_cast<HTML::Tokenizer::EndTagToken *>(&inToken)) {
		std::cout << "--" << endTagToken->TagName << std::endl;
	} else if (auto *doctypeToken = dynamic_cast<HTML::Tokenizer::DoctypeToken *>(&inToken)) {
		std::cout << "**doctype> " << doctypeToken->Name.value();
		if (doctypeToken->PublicIdentifier.has_value())
			std::cout << " PublicIdentifier='" << doctypeToken->PublicIdentifier.value();
		if (doctypeToken->SystemIdentifier.has_value())
			std::cout << " SystemIdentifier='" << doctypeToken->SystemIdentifier.value();
		std::cout << std::endl;
	} else if (auto *commentToken = dynamic_cast<HTML::Tokenizer::CommentToken *>(&inToken)) {
		std::cout << "**comment> '" << commentToken->Contents << '\'' << std::endl;
	}

	// Should really just create a mapped table with function pointers...
	switch (inToken.Type) {
		case  HTML::Tokenizer::TokenType::DOCTYPE: {
			auto *doctypeToken = dynamic_cast<HTML::Tokenizer::DoctypeToken *>(&inToken);
			bool quirky = IsQuirkyDoctype(doctypeToken);
			std::cout << "\033[1;31m -> DOCTYPE TYPE! HasQuirks: \033[1;35m" << (quirky ? "true" : "false") << "\033[1;0m" << std::endl;
		} break;
		default:
			// std::cout << " -> UnknownType: " << inToken.Type << std::endl;
			break;
	}
}
