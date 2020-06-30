#ifndef EMBER_EVENTS_H
#define EMBER_EVENTS_H

#include <SDL.h>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <string>
#include "SDLEmberInitializer.h"

#define LeftButton 1
#define MiddleButton 2
#define RightButton 3

namespace Ember
{
    enum class EventFlags
    {
        ESCAPE_CLOSE, RESIZEABLE, MEMORY
    };
    struct EventListenerHandle
    {
        unsigned int id;
    };

    template <typename T>
    class EventHandler
    {
    public:

        using idType = unsigned int;

        static EventListenerHandle registerListener(std::function<bool(const T&)> listener)
        {
            auto id = getNextId();
            getHandlers()[id] = listener;
            return EventListenerHandle{ id };
        }

        static void removeListener(EventListenerHandle handle)
        {
            getHandlers().erase(handle.id);
        }

        static void tringgerEvent(T e)
        {
            for (const auto& h : getHandlers())
            {
                h.second(e);
            }
        }

    private:
        static idType getNextId()
        {
            static idType id = 0;
            return ++id;
        }

        static std::unordered_map<idType, std::function<bool(const T&)>>& getHandlers()
        {
            static std::unordered_map<idType, std::function<bool(const T&)>> handlers;
            return handlers;
        }
    };

    template<typename T>
    class EventHandler<T*> {};

    template<typename T>
    class EventHandler<T&> {};

    template <typename T>
    class EventHandler<const T> {};

    template <typename T>
    void dispatchEvent(const T& t)
    {
        EventHandler<T>::tringgerEvent(t);
    }

    template <typename T>
    void dispatchEvent(T* t)
    {
        dispatchEvent(*t);
    }
    struct KeyBoardEvents
    {
        static bool Pressed;
        static bool Released;
        static int Repeat;
        static std::string Name;
    };
    struct MouseEvents
    {
        static int x, y;
        static bool Down;
        static bool Up;
        static int Id;
        static int Clicks;
    };
    struct MouseMotionEvents
    {
        static int XDirection;
        static int YDirection;
    };
    struct MouseWheelEvents
    {
        static int XDirection;
        static int YDirection;
    };

    static MouseEvents mouseEvent;
    static KeyBoardEvents keyBoardEvent;
    static MouseMotionEvents mouseMotionEvent;
    static MouseWheelEvents mouseWheelEvent;

    void initialize_events();
    bool iterate_events(SDLProperties& prop);
    void add_event_flags(EventFlags Flags);
}

#endif // !EMBER_EVENTS_H