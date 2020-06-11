/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

/*
 * TODO:
 *
 * In a lot of DOCTYPE handlers I do EmitDoctypeQuirksToken(),
 * but I should instead change the current doctype's flag Force-Quircks
 * to true
 */

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "ccompat.hpp"
#include "data/text/encoding/encoder_engine.hpp"
#include "data/text/encoding/utf8.hpp"
#include "data/text/named_characters.hpp"
#include "logger.hpp"
#include "misc/credits.hpp"
#include "net/alpn_protocols.hpp"
#include "net/http/http_connection.hpp"
#include "net/http2/http2_connection.hpp"
#include "options.hpp"
#include "parser/html/tokenizer.hpp"
#include "rendering/drawables/draw_rect.hpp"
#include "rendering/drawables/draw_text.hpp"
#include "rendering/opengl/gl_renderer.hpp"
#include "rendering/window/window_glfw.hpp"
#include "resources/document.hpp"

inline bool
DecodeText(Resources::DocumentResource *documentResource, std::vector<char> inputData) {
	auto charset = documentResource->mediaType.parameters.find("charset");
	if (charset == documentResource->mediaType.parameters.end()) {
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

		documentResource->data = Unicode::UString(utf8Encoding.Output);
		return true;
	}

	Logger::Warning("TextDecoder", "Unknown charset: " + charset->second);
	return false;
}

inline void
RunDocumentTest() {
	Resources::DocumentResource document;
	document.mediaType = { "text/html", { { "charset", "utf-8" } } };

	std::string testDocumentText = "<!-- TestHTML Document -->\n\
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
	std::vector<char> testDocument(std::begin(testDocumentText), std::end(testDocumentText));

	if (!DecodeText(&document, testDocument)) {
		Logger::Error("RunDocumentTest", "Failed to decode text");
		return;
	}

	HTML::ParserContext context;
	HTML::Tokenizer::Tokenizer tokenizer(context);
	tokenizer.Run(document);
}

inline std::shared_ptr<Rendering::Renderer>
CreateRenderer(const std::vector<Rendering::RendererType> &supportedRenderers) {
	if (std::find(std::cbegin(supportedRenderers), std::cend(supportedRenderers), Rendering::RendererType::OPENGL)
		!= std::end(supportedRenderers)) {
		return std::make_shared<Rendering::GLRenderer>();
	}

	return nullptr;
}

void
RunRenderingTest() {
	std::shared_ptr<Rendering::WindowBase> window;
	std::shared_ptr<Rendering::Renderer> renderer;

	// TODO Check for available windowing systems somehow
	window = std::make_shared<Rendering::WindowGLFW>();
	renderer = CreateRenderer(window->GetSupportedRenderers());

	if (renderer == nullptr) {
		Logger::Severe("RunRenderingTest", "No supported renderer for window "
										   "system "
											   + window->windowManagerName);
		return;
	}

	auto result = window->PrepareForRenderer(renderer->type);
	if (!result.first) {
		Logger::Severe("RunRenderingTest", "The creation of renderer context "
										   "for renderer for window system "
											   + window->windowManagerName + " failed.");
		return;
	}

	std::shared_ptr<Rendering::DrawRect> rectangle = std::make_shared<Rendering::DrawRect>();
	rectangle->bounds = { 0, 200, 0, 200 };
	rectangle->color.value = 0x83ff08ff;

	std::shared_ptr<Rendering::DrawText> text = std::make_shared<Rendering::DrawText>();
	text->bounds = { 300, 600, 300, 600 };
	text->color.value = 0x8308ffff;
	text->text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed\
 do eiusmod tempor incididunt ut labore et dolore magna aliqua.";

	renderer->SetWindow(window);
	renderer->Prepare();

	renderer->Enqueue(rectangle.get());
	renderer->Enqueue(text.get());

	while (!window->PollClose()) {
		renderer->DrawFrame();
	}

	renderer->Dequeue(text.get());
	renderer->Dequeue(rectangle.get());
}

void
RunNetHTTP2Test(const char *name) {
	Net::ConnectionInfo connectInfo(name, 443, true);
	connectInfo.tlsALPNProtocols = Net::ALPNProtocols::http2;
	if (!connectInfo.Connect()) {
		std::stringstream information;
		information << "Failed to connect! Host: \"" << connectInfo.hostName << "\":" << connectInfo.port;
		Logger::Error("HTTPConnection", information.str());
		return;
	}

	Net::HTTP::HTTP2Connection connection(&connectInfo);
	Net::HTTP::HTTPResponseInfo response;
	Net::HTTP::HTTP2Error error = connection.RequestNavigation(&response, "/");
	std::cout << "Error: " << error << "\nVersion: " << response.httpVersion << "\nStatusCode: " << response.statusCode
			  << "\nReasonPhrase: " << response.reasonPhrase << "\nHeaders: " << response.headers.size() << std::endl;

	for (const auto &headerField : response.headers) {
		std::cout << "\t\"" << headerField.fieldName << "\" = \"" << headerField.fieldValue << '\"' << std::endl;
	}

	std::string start = "============ Message Body ============";
	std::string end = "======================================";
	std::cout << "MessageBodySize: " << response.messageBody.size() << std::endl
			  << start << std::string(response.messageBody.data(), response.messageBody.size()) << '\n'
			  << end << std::endl;
}

int
main(int argc, const char *argv[]) {
	/** Initialization Section **/
	if (!Options::ParseCommandLine(argc, argv)) {
		return EXIT_FAILURE;
	}

	if (Options::GetCommandLineParameter("credits").has_value()) {
		Credits::PrintAuthorsToCommandLine();
		return EXIT_SUCCESS;
	}

	if (Options::GetCommandLineParameter("copyright").has_value()) {
		Credits::PrintCopyrightToCommandLine();
		return EXIT_SUCCESS;
	}

	NamedCharacters::Setup();

	/** Execution Section **/
	if (Options::GetCommandLineParameter("gui").has_value()) {
		RunRenderingTest();
		return EXIT_SUCCESS;
	}

	RunDocumentTest();

	/** Shutdown Section **/
	CCompat::CloseStandardIO();

	return EXIT_SUCCESS;
}
