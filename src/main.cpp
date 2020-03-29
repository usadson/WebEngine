/*
 * TODO:
 * 
 * In a lot of DOCTYPE handlers I do EmitDoctypeQuirksToken(),
 * but i should instead change the current doctype's flag Force-Quircks
 * to true
 */

#include <iostream>
#include <vector>


#include "ccompat.hpp"
#include "net/global.hpp"
#include "net/http/http_connection.hpp"
#include "parser/html/tokenizer.hpp"

const char TestDocument[] = "<!doctype html>\n\
<html>\n\
  <head>\n\
    <title>First title yeah</title>\n\
    <title>Title2</title>\n\
  </head>\n\
  <body>\n\
    <h1>Text Header</h1>\n\
    <hr />\n\
    <img src=\"logo.svg\" alt=\"Logo Image\" siz=d />\n\
  </body>\n\
</html>\n\
";

inline std::vector<char> VectorizeString(const char *text, size_t size) noexcept {
	return std::vector<char>(text, text + size);
}

void RunDoctypeTests() {

	std::vector<std::string> strings = {
		/*
		"<!-- Good comment -->",
		"<!-- Bad comment --!>",
		"<!-- <!-- nested --> -->",
		*/
		"<!DOCTYPE html>",
		"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">",
		"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">",
		"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Frameset//EN\" \"http://www.w3.org/TR/html4/frameset.dtd\">",
		"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">",
		"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">",
		"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Frameset//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd\">",
		"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">", 
		"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\">",
		"<!DOCTYPE html PUBLIC \"GOOD\">",
		"<!DOCTYPE html PUBLIC \"\" \"http://www.ibm.com/data/dtd/v11/ibmxhtml1-transitional.dtd\">",
		"<!DOCTYPE test>",
		"<hr />",
		// Error cases:
		"<!DOCTYPE html SYSTEM http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">",
		"<!DOCTYPE html PUBLIC \"-//IETF//DTD HTML//\">",
	};

	for (auto &string : strings) {
		HTML::Tokenizer::Tokenizer tokenizer;
		std::cout << "================ New Sequence ================" << std::endl;
		tokenizer.Run(VectorizeString(string.c_str(), string.length()));
	}
}

inline void RunDocumentTest(void) {
	HTML::Tokenizer::Tokenizer tokenizer;
	tokenizer.Run(VectorizeString(TestDocument, sizeof(TestDocument)-1));
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

int main(int argc, char *argv[]) {
	if (argc == 1) {
		std::cerr << "Please specify a domain!" << std::endl;
		return EXIT_FAILURE;
	}
	// RunDoctypeTests();
	// RunDocumentTest();
	RunNetTest(argv[1]);

	// Just for valgrind:
	CCompat::CloseStandardIO();
	return EXIT_SUCCESS;
}
