#pragma once

#include <atomic>

class atomic_bool_wrapper
{
    public:

        explicit atomic_bool_wrapper( bool initial_state )
            : m_value( initial_state )
        {

        }

    public:

        bool is_true() const
        {
            return m_value == true;
        }

        bool is_false() const
        {
            return m_value == false;
        }

    public:

        void set_true()
        {
            m_value = true;
        }

        void set_false()
        {
            m_value = false;
        }

    private:

        std::atomic_bool m_value;
};
