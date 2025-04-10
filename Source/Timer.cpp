#include "Timer.hpp"

#include <Windows.h>

namespace CatalystZero
{
    Timer::Timer()
        : m_DeltaTime(0.01666f)
    {
        LARGE_INTEGER* lastCounter = reinterpret_cast<LARGE_INTEGER*>(&m_LastCounter);
        LARGE_INTEGER* frequencyCounterLarge = reinterpret_cast<LARGE_INTEGER*>(&m_FrequencyCounterLarge);

        QueryPerformanceCounter(lastCounter);
        QueryPerformanceFrequency(frequencyCounterLarge);
        m_FrequencyCounter = static_cast<float>(frequencyCounterLarge->QuadPart);
    }
    
    void Timer::Tick()
    {
        LARGE_INTEGER currentCounter;
        QueryPerformanceCounter(&currentCounter);

        LARGE_INTEGER* lastCounter = reinterpret_cast<LARGE_INTEGER*>(&m_LastCounter);
        m_DeltaTime = static_cast<float>(currentCounter.QuadPart - lastCounter->QuadPart) / m_FrequencyCounter;
        *lastCounter = currentCounter;
    }
}