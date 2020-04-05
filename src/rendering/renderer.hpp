#pragma once

#include "render_object.hpp"

namespace Rendering {

	enum class RendererType {
		OPENGL
	};

	class Renderer {
	public: // Properties
		const RendererType Type;
	public: // Methods
		inline Renderer(RendererType type)
			: Type(type) {
		}

		virtual void Enqueue(RenderObject *object) = 0;
	};

}
