#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include <vector>

#include "distribution.hpp"
#include "uncopyable.hpp"

class table : public uncopyable
{
    public:

        explicit table( int seed )
            : m_distribution( seed )
        {
            int table_size = static_cast< int >( 'z' ) - static_cast< int >( 'a' ) + 1;

            m_table.resize( table_size, { 0, 0 } );
        }

    public:

        int get( int idx ) const
        {
            return m_table[ idx ][ 0 ];
        }

        int size() const
        {
            return static_cast< int >( m_table.size() );
        }

        int sum() const
        {
            int result = 0;

            for ( int i = 0; i < size(); ++i )
                result += m_table[ i ][ 0 ];

            return result;
        }

        int error() const
        {
            int result = 0;

            for ( int i = 0; i < size(); ++i )
                result += std::abs( m_table[ i ][ 0 ] - m_table[ i ][ 1 ] );

            return result;
        }

    public:

        void reset()
        {
            for ( int i = 0; i < size(); ++i )
                m_table[ i ][ 0 ] = 0;
        }

        void backup()
        {
            for ( int i = 0; i < size(); ++i )
                m_table[ i ][ 1 ] = m_table[ i ][ 0 ];
        }

        void update( const std::string &sentence )
        {
            reset();

            for ( auto &c : sentence )
            {
                if ( c < 'a' || c > 'z' )
                    continue;

                int idx = static_cast< int >( c ) - static_cast< int >( 'a' );

                m_table[ idx ][ 0 ]++;
            }
        }

        void random( int offset )
        {
            if ( offset == 0 )
            {
                for ( int i = 0; i < size(); ++i )
                    m_table[ i ][ 0 ] = std::max( 0, m_distribution( 100 ) - 80 );
            }
            else
            {
                for ( int i = 0; i < size(); ++i )
                    m_table[ i ][ 0 ] = m_distribution( 50 ) + offset;
            }
        }

    private:

        distribution m_distribution;

        std::vector< std::array< int, 2 > > m_table;
};
