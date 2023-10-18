#pragma once

#include "atomic_bool_wrapper.hpp"
#include "uncopyable.hpp"

class execution_state : public uncopyable
{
    public:

        explicit execution_state()
            : m_state( nullptr )
            , m_is_local( false )
        {
            ptr_alloc();
        }

    public:

        ~execution_state()
        {
            ptr_free();
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

        void set( atomic_bool_wrapper *state )
        {
            ptr_alloc( state );
        }

    private:

        void ptr_alloc( atomic_bool_wrapper *state = nullptr )
        {
            ptr_free();

            if ( state == nullptr )
            {
                m_state    = new atomic_bool_wrapper( true );
                m_is_local = true;
            }
            else
            {
                m_state    = state;
                m_is_local = false;
            }
        }

        void ptr_free()
        {
            if ( m_is_local )
            {
                delete m_state;

                m_state    = nullptr;
                m_is_local = false;
            }
            else
            {
                m_state    = nullptr;
                m_is_local = false;
            }
        }

    private:

        atomic_bool_wrapper *m_state;

        bool m_is_local;
};
