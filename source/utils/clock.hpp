#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

#include "uncopyable.hpp"

class clock : public uncopyable
{
    public:

        static void tic()
        {
            if ( !QueryPerformanceCounter( &m_save_time ) )
                m_save_time.QuadPart = 0;
        }

        static double toc()
        {
            LARGE_INTEGER time;

            set_frequency();

            if ( !QueryPerformanceCounter( &time ) )
                time.QuadPart = 0;

            if ( time.QuadPart != 0 && m_save_time.QuadPart != 0 && m_frequency.QuadPart != 0 )
            {
                double n = static_cast< double >( time.QuadPart - m_save_time.QuadPart );
                double d = static_cast< double >( m_frequency.QuadPart );

                return n / d;
            }
            else
                return 0.0;
        }

    private:

        static void set_frequency()
        {
            if ( m_frequency.QuadPart != 0 )
                return;

            if ( !QueryPerformanceFrequency( &m_frequency ) )
                m_frequency.QuadPart = 0;
        }

    private:

        inline static thread_local LARGE_INTEGER m_save_time = { 0, 0 };
        inline static thread_local LARGE_INTEGER m_frequency = { 0, 0 };
};
