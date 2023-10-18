#pragma once

#include <random>

#include "uncopyable.hpp"

class distribution : public uncopyable
{
    public:

        explicit distribution( int seed )
        {
            m_number_generator.seed( seed );

            m_distribution = std::uniform_int_distribution< int >( 0, 10000000 );
        }

    public:

        int operator ()( int max_value )
        {
            return m_distribution( m_number_generator ) % max_value;
        }

    private:

        std::mt19937 m_number_generator;

        std::uniform_int_distribution< int > m_distribution;
};
