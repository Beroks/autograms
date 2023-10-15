#pragma once

#include <atomic>

#include "uncopyable.hpp"

class execution_state : public uncopyable
{
    public:

        explicit execution_state( std::atomic< bool > *state )
            : m_state( state )
            , m_is_local( state == nullptr )
        {
            if ( m_state == nullptr )
                m_state = new std::atomic< bool >();

            *m_state = true;
        }

    public:

        ~execution_state()
        {
            if ( m_is_local )
                delete m_state;
        }

    public:

        bool is_running() const
        {
            return *m_state;
        }

    public:

        void stop()
        {
            *m_state = false;
        }

    private:

        std::atomic< bool > *m_state;

        bool m_is_local;
};
