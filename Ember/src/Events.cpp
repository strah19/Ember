#include "Events.h"

namespace Ember
{
    bool CloseWithEscape = false;
    bool Resize = false;

    bool KeyBoardEvents::Pressed;
    bool KeyBoardEvents::Released;
    std::string KeyBoardEvents::Name;
    int KeyBoardEvents::Repeat;

    int MouseEvents::x;
    int MouseEvents::y;
    int MouseEvents::Id;
    bool MouseEvents::Down;
    bool MouseEvents::Up;
    int MouseEvents::Clicks;

    int MouseMotionEvents::XDirection;
    int MouseMotionEvents::YDirection;

    int MouseWheelEvents::XDirection;
    int MouseWheelEvents::YDirection;

    void initialize_events()
    {
        auto keys = EventHandler<KeyBoardEvents>::registerListener([](const KeyBoardEvents& ev)->bool
            {
                return false;
            });
        auto mouse = EventHandler<MouseEvents>::registerListener([](const MouseEvents& ev)->bool
            {
                return false;
            });
        auto motion = EventHandler<MouseMotionEvents>::registerListener([](const MouseMotionEvents& ev)->bool
            {
                return false;
            });
        auto wheel = EventHandler<MouseWheelEvents>::registerListener([](const MouseWheelEvents& ev)->bool
            {
                return false;
            });
    }
    bool iterate_events(SDLProperties& prop)
    {
        SDL_Event* event = &prop.Event;
        switch (event->type)
        {
        case SDL_KEYDOWN: {
            keyBoardEvent.Name = SDL_GetKeyName(event->key.keysym.sym);
            keyBoardEvent.Pressed = true;
            keyBoardEvent.Released = false;
            keyBoardEvent.Repeat = event->key.repeat;
            if (CloseWithEscape) {
                switch (event->key.keysym.sym) {
                    case SDLK_ESCAPE: {
                        return false;
                        break;
                    }
                }
            }
            dispatchEvent(keyBoardEvent);
            break;
        }
        case SDL_KEYUP: {
            keyBoardEvent.Pressed = false;
            keyBoardEvent.Released = true;
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            mouseEvent.Down = true;
            mouseEvent.Up = false;
            mouseEvent.Id = event->button.button;
            mouseEvent.Clicks = event->button.clicks;
            dispatchEvent(mouseEvent);
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            mouseEvent.Down = false;
            mouseEvent.Up = true;
            dispatchEvent(mouseEvent);
            break;
        }
        case SDL_MOUSEMOTION: {
            mouseMotionEvent.XDirection = event->motion.x;
            mouseMotionEvent.YDirection = event->motion.y;
            dispatchEvent(mouseMotionEvent);
            break;
        }
        case SDL_MOUSEWHEEL: {
            mouseWheelEvent.XDirection = event->wheel.x;
            mouseWheelEvent.YDirection = event->wheel.y;
            dispatchEvent(mouseWheelEvent);
            break;
        }
        case SDL_QUIT:{
            return false;
            break;
        }
        default:
            break;
        }
        mouseEvent.x = event->button.x;
        mouseEvent.y = event->button.y;
        if (Resize)
            SDL_SetWindowResizable(prop.Window, SDL_TRUE);

        return true;
    }
    void add_event_flags(EventFlags Flags)
    {
        if (Flags == EventFlags::ESCAPE_CLOSE) {
            CloseWithEscape = true;
        }
        Resize = (Flags == EventFlags::RESIZEABLE) ? true : false;
    }
}