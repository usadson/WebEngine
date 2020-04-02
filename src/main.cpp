/*
 * TODO:
 * 
 * In a lot of DOCTYPE handlers I do EmitDoctypeQuirksToken(),
 * but i should instead change the current doctype's flag Force-Quircks
 * to true
 */

#include <iostream>
#include <vector>

#include "data/text/named_characters.hpp"
#include "data/text/encoding/utf8.hpp"
#include "net/global.hpp"
#include "net/http/http_connection.hpp"
#include "parser/html/tokenizer.hpp"
#include "resources/document.hpp"
#include "ccompat.hpp"
#include "logger.hpp"

const char TestDocument[] = "<!doctype html>\n\
<html>\n\
  <head>\n\
    <title>First title yeah</title>\n\
    <title>Title2</title>\n\
  </head>\n\
  <body>\n\
    <h1>Text Header</h1>\n\
    <p>Paragraph start tag: &lt;p&gt;</p>\n\
    <hr />\n\
    <img src=\"logo.svg\" alt=\"Logo Image\" siz=d />\n\
  </body>\n\
</html>\n\
";

inline std::vector<char> VectorizeString(const char *text, size_t size) noexcept {
	return std::vector<char>(text, text + size);
}

void RunNetTest(const char *name) {
	Net::Global::SetupTLS();

	Net::ConnectionInfo connectInfo(name, 443, true);
	Net::HTTP::HTTPConnection connection(connectInfo);
	Net::HTTP::HTTPResponseInfo response;
	Net::HTTP::HTTPConnectionError error = connection.RequestNavigation(&response, "/");
	std::cout << "Error: " << error
			  << "\nVersion: " << response.HTTPVersion
			  << "\nStatusCode: " << response.StatusCode
			  << "\nReasonPhrase: " << response.ReasonPhrase 
			  << "\nHeaders: " << response.Headers.size()
			  << std::endl;
	for (const auto &headerField : response.Headers) {
		std::cout << "\t\"" << headerField.FieldName << "\" = \"" << headerField.FieldValue << '\"' << std::endl;
	}
	std::cout << "MessageBodySize: " << response.MessageBody.size() << std::endl;
	std::string start = "============ Message Body ============";
	std::string end   = "======================================";
	std::cout << start << std::string(response.MessageBody.data(), response.MessageBody.size()) << '\n' << end << std::endl;

	Net::Global::DestroyTLS();
}

inline bool DecodeText(Resources::DocumentResource &documentResource, std::vector<char> inputData) {
	auto charset = documentResource.Mime.Parameters.find("charset");
	if (charset == documentResource.Mime.Parameters.end()) {
		Logger::Warning("TextDecoder", "TODO: Add charset/encoding sniffing.");
		return false;
	}

	// Labels: https://encoding.spec.whatwg.org/#names-and-labels
	if (charset->second == "utf-8" || charset->second == "utf8" || charset->second == "unicode-1-1-utf-8") {
		TextEncoding::UTF8 utf8Encoding;
		if (!utf8Encoding.Decode(inputData.data(), inputData.size())) {
			Logger::Warning("TextDecoder", "Failed to decode text!");
			return false;
		}
		documentResource.Data = utf8Encoding.Output;
		return true;
	}

	Logger::Warning("TextDecoder", "Unknown charset: " + charset->second);
	return false;
}

inline void RunDocumentTest(void) {
	Resources::DocumentResource document;
	document.Mime = { "text/html", { { "charset", "utf-8" } } };

	if (!DecodeText(document, VectorizeString(TestDocument, sizeof(TestDocument) / sizeof(TestDocument[0]) - 1))) {
		Logger::Error("RunDocumentTest", "Failed to decode text");
		return;
	}

	HTML::Tokenizer::Tokenizer tokenizer;
	tokenizer.Run(document);
}

void RunEncodingTest() {
	std::vector<std::string> strings = {
		"κόσμε",
		"🧒",
		"اَلْعَرَبِيَّةُ",	
	};

	TextEncoding::UTF8 utf8Encoding;
	for (const auto &string : strings) {
		std::vector<char> vector = VectorizeString(string.c_str(), string.length());
		bool result = utf8Encoding.Decode(vector.data(), vector.size());
		std::cout << "Result=" << (result ? "passed" : "failed") << ": ";
		for (const auto &c : utf8Encoding.Output) {
			std::cout << std::hex << "U+" << c << ' ';
		}
		std::cout << std::endl;
	}
}

int main(int argc, char *argv[]) {
	NamedCharacters::Setup();
	/*
	if (argc == 1) {
		std::cerr << "Please specify a domain!" << std::endl;
		return EXIT_FAILURE;
	}
	*/
// 	RunDoctypeTests();
	RunDocumentTest();
// 	RunNetTest(argv[1]);
// 	RunEncodingTest();

	// Just for valgrind:
	CCompat::CloseStandardIO();
	return EXIT_SUCCESS;
}
