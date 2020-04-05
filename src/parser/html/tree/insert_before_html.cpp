#include "insert_before_html.hpp"

#include <iostream>
#include <vector>

#include "dom/comment.hpp"
#include "dom/element.hpp"
#include "parser/html/constants.hpp"
#include "logger.hpp"

bool HTML::InsertionModes::BeforeHTML::EmitToken(HTML::Tokenizer::Token &inToken) {
	HTML::Tokenizer::CharacterToken *characterToken;
	HTML::Tokenizer::CommentToken	*commentToken;
	HTML::Tokenizer::StartTagToken	*startTagToken;

	switch (inToken.Type) {
		case HTML::Tokenizer::TokenType::DOCTYPE:
			// Ignore token
			return false;
		case HTML::Tokenizer::TokenType::COMMENT:
			commentToken = dynamic_cast<HTML::Tokenizer::CommentToken *>(&inToken);
			Context.ParserContext.DocumentNode.ChildNodes.push_back(DOM::Comment(commentToken->Contents));
			return false;
		case HTML::Tokenizer::TokenType::CHARACTER:
			characterToken = dynamic_cast<HTML::Tokenizer::CharacterToken *>(&inToken);
			switch (characterToken->Character) {
				case Unicode::CHARACTER_TABULATION:
				case Unicode::LINE_FEED:
				case Unicode::FORM_FEED:
				case Unicode::CARRIAGE_RETURN:
				case Unicode::SPACE:
					// Ignore this token:
					return false;
				default:
					break;
			}
			break;
		case HTML::Tokenizer::TokenType::STARTTAG:
			startTagToken = dynamic_cast<HTML::Tokenizer::StartTagToken *>(&inToken);

			if (startTagToken->TagName.EqualsA("html")) {
				DOM::Element element;
				element.NamespaceURI = HTML::Constants::HTMLNamespace;
				element.LocalName = startTagToken->TagName;
				element.Document = Context.ParserContext.DocumentNode;

				for (auto const &attribute : startTagToken->Attributes) {
					element.InternalAttributes.insert(attribute);
				}

				
				return false;
			}

			/* Fallthrough */
		default:
			break;
	}

	return true;
}
