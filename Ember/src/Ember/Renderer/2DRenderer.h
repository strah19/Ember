#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include "../Platform/Window.h"
#include "../Structures/Color.h"

namespace ember {
    class Renderer2D {
    public:
        Renderer2D(Window* window);
        virtual ~Renderer2D();

        virtual SDL_Renderer* Renderer();
        virtual bool Initializer();
        virtual void Destroy();

        virtual void SetViewport(int x, int y, int w, int h);

        virtual void Clear(Color& color);
        virtual void SetColor(Color& color);
        virtual void Show();
    private:
        SDL_Renderer* renderer;
        Window* window;
    };
}

#endif // !SDL_RENDERER_H
