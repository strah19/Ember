#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include "../Renderer2D.h"
#include "../../Platform/Window.h"

#include <SDL.h>

namespace ember {
    class SDLRenderer : public Renderer2D {
    public:
        SDLRenderer(Window* window);
        virtual ~SDLRenderer();

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
