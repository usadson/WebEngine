/*
 * TODO:
 * 
 * In a lot of DOCTYPE handlers I do EmitDoctypeQuirksToken(),
 * but i should instead change the current doctype's flag Force-Quircks
 * to true
 */

#include <iostream>
#include <memory>
#include <vector>

#include "data/text/named_characters.hpp"
#include "data/text/encoding/encoder_engine.hpp"
#include "data/text/encoding/utf8.hpp"
#include "net/global.hpp"
#include "net/http/http_connection.hpp"
#include "parser/html/tokenizer.hpp"
#include "rendering/opengl/gl_renderer.hpp"
#include "rendering/window/window_glfw.hpp"
#include "resources/document.hpp"
#include "ccompat.hpp"
#include "logger.hpp"

const char TestDocument[] = "<!-- TestHTML Document -->\n\
<!doctype html>\n\
<HTml>\n\
  <head>\n\
    <title>First title yeah</title>\n\
    <title>Title2</title>\n\
  </head>\n\
  <body>\n\
    <h1>Text Header</h1>\n\
    Stray Text?\n\
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

	HTML::ParserContext context;
	HTML::Tokenizer::Tokenizer tokenizer(context);
	tokenizer.Run(document);
}

void RunEncodingTest() {
	size_t i;

	std::vector<std::pair<std::string, std::vector<char>>> vector = {
		{
			"866",
			{
				(char) 0x80,
				(char) 0x81
			}
		},
		{
			"ascii",
			{
				'A',
				'B',
				'C',
			}
		},
		{
			"iso885910",
			{
				(char) 162
			},
		}
	};

	for (const auto &pair : vector) {
		std::vector<Unicode::CodePoint> out = TextEncoding::EncoderEngine::DecodeData(pair.second.data(), pair.second.size(), pair.first);

		std::cout << "Encoder output: " << out.size() << " code points (characters)." << std::endl;
		for (i = 0; i < out.size(); i++) {
			std::cout << '\t' << out[i] << std::endl;
		}
		std::cout << "End." << std::endl;
	}
}

std::shared_ptr<Rendering::Renderer> CreateRenderer(std::vector<Rendering::RendererType> supportedRenderers) {
	for (const auto &renderer : supportedRenderers) {
		switch (renderer) {
			case Rendering::RendererType::OPENGL:
				return std::make_shared<Rendering::GLRenderer>();
			default:
				break;
		}
	}

	return nullptr;
}

void RunRenderingTest() {
	std::shared_ptr<Rendering::WindowBase> window;
	std::shared_ptr<Rendering::Renderer> renderer;

	// TODO Check for available windowing systems somehow
	window = std::make_shared<Rendering::WindowGLFW>();
	renderer = CreateRenderer(window->GetSupportedRenderers());

	if (renderer == nullptr) {
		Logger::Severe("RunRenderingTest", "No supported renderer for window system " + window->WindowManagerName);
		return;
	}

	auto result = window->PrepareForRenderer(renderer->Type);
	if (!result.first) {
		Logger::Severe("RunRenderingTest", "The creation of renderer context for renderer for window system " + window->WindowManagerName + " failed.");
	}

	renderer->SetWindow(window);

	while (!window->PollClose()) {
		renderer->DrawFrame();
	}
}

int main(int argc, char *argv[]) {
	NamedCharacters::Setup();

// 	RunDoctypeTests();
// 	RunDocumentTest();
// 	RunNetTest(argv[1]);
// 	RunEncodingTest();
	RunRenderingTest();

	// Just for valgrind:
	CCompat::CloseStandardIO();
	return EXIT_SUCCESS;
}
