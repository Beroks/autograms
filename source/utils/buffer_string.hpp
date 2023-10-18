#pragma once

#include <string>

#include "uncopyable.hpp"

class buffer_string : public uncopyable
{
    public:

        buffer_string()
        {
            m_buffer.reserve( 1000 );
        }

    public:

        bool empty() const
        {
            return m_buffer.empty();
        }

        const std::string &value() const
        {
            return m_buffer;
        }

    public:

        template< class type >
        buffer_string &operator =( const type &data )
        {
            m_buffer = data;

            return *this;
        }

        template< class type >
        buffer_string &operator <<( const type &data )
        {
            m_buffer += data;

            return *this;
        }

    private:

        std::string m_buffer;
};
