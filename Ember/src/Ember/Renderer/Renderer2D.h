#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "../Structures/Color.h"

namespace ember {
	class Renderer2D {
	public:
		virtual ~Renderer2D() = default;

        virtual bool Initializer() = 0;
        virtual void Destroy() = 0;

		virtual void SetViewport(int x, int y, int w, int h) = 0;
		virtual void Clear(Color& color) = 0;
		virtual void Show() = 0;
	};
}

#endif // !RENDERER_2D_H
