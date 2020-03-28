#include "tree_constructor.hpp"

#include <memory>
#include <iostream>

namespace HTML {

	void TreeConstructor::EmitToken(HTML::Tokenizer::Token &inToken) {
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
			case  HTML::Tokenizer::TokenType::DOCTYPE:
				std::cout << "\033[1;31m -> DOCTYPE TYPE!!!\033[1;0m" << std::endl;
				break;
			default:
				// std::cout << " -> UnknownType: " << inToken.Type << std::endl;
				break;
		}
	}

	void TreeConstructor::EmitCharacterToken(char character) {
		HTML::Tokenizer::CharacterToken token(character);
		EmitToken(token);
	}

	void TreeConstructor::EmitDoctypeQuirksToken(void) {
		HTML::Tokenizer::DoctypeToken token;
		token.ForceQuirks = true;
		EmitToken(token);
	}

	void TreeConstructor::EmitEOFToken(void) {
		HTML::Tokenizer::EOFToken token;
		EmitToken(token);
	}
};
 
