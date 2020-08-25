#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include "../Platform/Window.h"
#include "../Structures/Color.h"
#include "../Structures/Rect.h"

namespace ember {
    class Renderer2D {
    public:
        Renderer2D(Window* window);
        virtual ~Renderer2D();

         SDL_Renderer* Renderer();
        bool Initializer();
        void Destroy();

        void SetViewport(int x, int y, int w, int h);

        void Clear(const Color& color);
        void SetColor(const Color& color);
        void Show();

        void Rectangle(const Rect& rect, const Color& color);
        void Line(const IVec2& point1, const IVec2& point2, const Color& color);
        void Point(const IVec2& point, const Color& color);

        void RectangleF(const FRect& rect, const Color& color);
        void LineF(const Vec2& point1, const Vec2& point2, const Color& color);
        void PointF(const Vec2& point, const Color& color);
    private:
        SDL_Renderer* renderer;
        Window* window;
    };
}

#endif // !SDL_RENDERER_H
