#include "token.hpp"

#include <iostream>

#include "logger.hpp"

namespace HTML {
	namespace Tokenizer {
		const AmbiguousTagToken AmbiguousTagToken::INVALID_TYPE = AmbiguousTagToken(TokenType::INVALID);
		const CommentToken CommentToken::INVALID_TYPE = CommentToken("INVALID");
		const EndTagToken EndTagToken::INVALID_TYPE = EndTagToken();
		const StartTagToken StartTagToken::INVALID_TYPE = StartTagToken();

		void AmbiguousTagToken::AddTokenAttribute(HTML::Tokenizer::Context &context) {
			Logger::Debug("AmbiguousTagToken::AddTokenAttribute", "TryINSERT");
			std::cout << "AttributeName: " << AttributeName << "\nAttributeValue: " << AttributeValue << std::endl;
			if (!Attributes.insert(std::make_pair(AttributeName, AttributeValue)).second) {
				context.LogError(HTML::Tokenizer::ParserError::DUPLICATE_ATTRIBUTES);
			}
			Logger::Debug("AmbiguousTagToken::AddTokenAttribute", "EndINSERT");

			// Reset:
			AttributeName = "";
			AttributeValue = "";
		}
	};
};
