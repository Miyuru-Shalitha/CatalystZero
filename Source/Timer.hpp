#pragma once

namespace CatalystZero
{
    class Timer
    {
    public:
        Timer();
        
        void Tick();
        
        inline float GetDeltaTime() const { return m_DeltaTime; }
        
    private:
        struct LargeInteger
        {
            unsigned long HighPart;
            long LowParg;
        } m_LastCounter, m_FrequencyCounterLarge;
        float m_FrequencyCounter;
        float m_DeltaTime;
    };
}