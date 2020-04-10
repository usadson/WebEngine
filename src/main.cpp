/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

/*
 * TODO:
 * 
 * In a lot of DOCTYPE handlers I do EmitDoctypeQuirksToken(),
 * but i should instead change the current doctype's flag Force-Quircks
 * to true
 */

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "data/text/named_characters.hpp"
#include "data/text/encoding/encoder_engine.hpp"
#include "data/text/encoding/utf8.hpp"
#include "net/http/http_connection.hpp"
#include "net/http2/http2_connection.hpp"
#include "net/alpn_protocols.hpp"
#include "parser/html/tokenizer.hpp"
#include "rendering/drawables/draw_rect.hpp"
#include "rendering/drawables/draw_text.hpp"
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
		return;
	}

	auto result = window->PrepareForRenderer(renderer->Type);
	if (!result.first) {
		Logger::Severe("RunRenderingTest", "The creation of renderer context for renderer for window system " + window->WindowManagerName + " failed.");
		return;
	}

	std::shared_ptr<Rendering::DrawRect> rectangle = std::make_shared<Rendering::DrawRect>();
	rectangle->Bounds = { 0, 200, 0, 200 };
	rectangle->Color.Value = 0x83ff08ff;

	std::shared_ptr<Rendering::DrawText> text = std::make_shared<Rendering::DrawText>();
	text->Bounds = { 300, 600, 300, 600 };
	text->Color.Value = 0x8308ffff;
	text->Text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed\
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

void RunNetHTTP2Test(const char *name) {
	Net::ConnectionInfo connectInfo(name, 443, true);
	connectInfo.TLSALPNProtocols = Net::ALPNProtocols::HTTP2;
	if (!connectInfo.Connect()) {
		std::stringstream information;
		information << "Failed to connect! Host: \"" << connectInfo.HostName << "\":" << connectInfo.Port;
		Logger::Error("HTTPConnection", information.str());
		return;
	}

	/**/
	Net::HTTP::HTTP2Connection connection(&connectInfo);
	Net::HTTP::HTTPResponseInfo response;
	Net::HTTP::HTTP2Error error = connection.RequestNavigation(&response, "/");
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
}

int main(int argc, char *argv[]) {
	NamedCharacters::Setup();

// 	RunDoctypeTests();
// 	RunDocumentTest();
// 	RunNetTest(argv[1]);
// 	RunEncodingTest();
// 	RunRenderingTest();

	if (argc == 1)
		Logger::Warning("OPT", "Please specify a domain to connect to.");
	else 
		RunNetHTTP2Test(argv[1]);

	// Just for valgrind:
	CCompat::CloseStandardIO();
	return EXIT_SUCCESS;
}
