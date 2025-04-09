#include "EventSystem.hpp"

namespace CatalystZero
{
    Event::Event(EventType type)
        : m_Type(type), m_Handled(false) { }
    
    WindowClosedEvent::WindowClosedEvent()
        : Event(EventType::WindowClosed) { }

    EventDispatcher::EventDispatcher(const Event& event)
        : m_Event(const_cast<Event&>(event)) { }    
}