#pragma once

#include <random>

#include "uncopyable.hpp"

class distribution : public uncopyable
{
    public:

        explicit distribution( int seed )
        {
            m_number_generator.seed( seed );

            m_distribution = std::uniform_int_distribution< int >( 0, 9 );
        }

    public:

        int operator ()( int offset = 0 )
        {
            return m_distribution( m_number_generator ) + offset;
        }

    private:

        std::mt19937 m_number_generator;

        std::uniform_int_distribution< int > m_distribution;
};
