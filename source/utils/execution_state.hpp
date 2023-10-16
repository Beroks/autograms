#pragma once

#include "atomic_bool_wrapper.hpp"
#include "uncopyable.hpp"

class execution_state : public uncopyable
{
    public:

        explicit execution_state( atomic_bool_wrapper *state )
            : m_state( state )
            , m_is_local( state == nullptr )
        {
            if ( m_is_local )
                m_state = new atomic_bool_wrapper( true );
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
            return m_state->is_true();
        }

    public:

        void stop()
        {
            m_state->set_false();
        }

    private:

        atomic_bool_wrapper *m_state;

        bool m_is_local;
};
