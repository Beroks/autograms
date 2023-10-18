#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "uncopyable.hpp"

class number_converter : public uncopyable
{
    public:

        number_converter()
        {
            initialize_lut();
        }

    public:

        int min_value() const
        {
            return 0;
        }

        int max_value() const
        {
            return static_cast< int >( m_lut.size() ) - 1;
        }

    public:

        const std::string &operator ()( int number ) const
        {
            if ( number < 0 || number > static_cast< int >( m_lut.size() ) - 1 )
                throw std::runtime_error( "number_converter : invalid input" );

            return m_lut[ number ];
        }

    private:

        void initialize_lut()
        {
            m_lut.resize( 100 );

            m_lut[ 0 ] = "zero";
            m_lut[ 1 ] = "one";
            m_lut[ 2 ] = "two";
            m_lut[ 3 ] = "three";
            m_lut[ 4 ] = "four";
            m_lut[ 5 ] = "five";
            m_lut[ 6 ] = "six";
            m_lut[ 7 ] = "seven";
            m_lut[ 8 ] = "eight";
            m_lut[ 9 ] = "nine";

            m_lut[ 10 ] = "ten";
            m_lut[ 11 ] = "eleven";
            m_lut[ 12 ] = "twelve";
            m_lut[ 13 ] = "thirteen";
            m_lut[ 14 ] = "fourteen";
            m_lut[ 15 ] = "fifteen";
            m_lut[ 16 ] = "sixteen";
            m_lut[ 17 ] = "seventeen";
            m_lut[ 18 ] = "eighteen";
            m_lut[ 19 ] = "nineteen";

            for ( int i = 20; i <= 29; ++i )
                m_lut[ i ] = "twenty";

            for ( int i = 30; i <= 39; ++i )
                m_lut[ i ] = "thirty";

            for ( int i = 40; i <= 49; ++i )
                m_lut[ i ] = "forty";

            for ( int i = 50; i <= 59; ++i )
                m_lut[ i ] = "fifty";

            for ( int i = 60; i <= 69; ++i )
                m_lut[ i ] = "sixty";

            for ( int i = 70; i <= 79; ++i )
                m_lut[ i ] = "seventy";

            for ( int i = 80; i <= 89; ++i )
                m_lut[ i ] = "eighty";

            for ( int i = 90; i <= 99; ++i )
                m_lut[ i ] = "ninety";

            for ( int i = 20; i <= 99; ++i )
                if ( i % 10 != 0 )
                    m_lut[ i ] += "-" + m_lut[ i % 10 ];
        }

    private:

        std::vector< std::string > m_lut;
};
