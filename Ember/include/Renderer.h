#ifndef RENDERER_H
#define RENDERER_H

#include "Ember.h"
#include "Window.h"

namespace Ember {
	class rRenderer {
	public:
		rRenderer(Window* window);
		virtual ~rRenderer();

		SDL_Renderer* Renderer();

		void SetViewport(int x, int y, int w, int h);

		void Clear(const Color& color);
		void SetColor(const Color& color);
		void Show();

		void Rectangle(const Rect& rect, const Color& color);
		void Border(const Rect& rect, const Color& color);
		void Line(const IVec2& point1, const IVec2& point2, const Color& color);
		void Point(const IVec2& point, const Color& color);
		void DrawCircle(const IVec2& position, int radius, const Color& color);
		void FillCircle(const IVec2& position, int radius, const Color& color);

		void RectangleF(const FRect& rect, const Color& color);
		void BorderF(const FRect& rect, const Color& color);
		void LineF(const Vec2& point1, const Vec2& point2, const Color& color);
		void PointF(const Vec2& point, const Color& color);

		void Curve(IVec2 pos[], const Color& color);
		void AdvCurve(IVec2 pos[], const Color& color);
	private:
		SDL_Renderer* renderer;
		Window* window;

		bool Initializer();
		void Destroy();
	};
}

#endif // !RENDERER_H
