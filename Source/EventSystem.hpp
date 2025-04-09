#pragma once

#include <functional>
#include <cassert>

namespace CatalystZero
{
    enum class EventType
    {
        WindowClosed
    };

    class Event
    {
    public:
        Event(EventType type);
        
        inline EventType GetType() const { return m_Type; }
        inline void SetIsHandled(bool handled) { m_Handled = handled; }

    private:
        EventType m_Type;
        bool m_Handled;
    };

    // NOTE(Miyuru): Maby I have to make a member varaible for window handle.
    class WindowClosedEvent : public Event
    {
    public:
        WindowClosedEvent();
        
        inline static EventType GetStaticType() { return EventType::WindowClosed; }
    };
    
    class EventDispatcher
    {
    public:
        EventDispatcher(const Event& event);
        
        template<typename T>
        void Dispatch(std::function<void(const T&)> callback)
        {
            if (T::GetStaticType() == m_Event.GetType())
            {
                assert(callback != nullptr);
                m_Event.SetIsHandled(true);
                callback(static_cast<const T&>(m_Event));
            }
        }
        
    private:
        Event& m_Event;
    };
}