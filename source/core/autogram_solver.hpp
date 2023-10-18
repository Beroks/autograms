#pragma once

#include <algorithm>
#include <string>

#include "../utils/atomic_bool_wrapper.hpp"
#include "../utils/buffer_string.hpp"
#include "../utils/execution_state.hpp"
#include "../utils/number_converter.hpp"
#include "../utils/table.hpp"
#include "../utils/uncopyable.hpp"

class autogram_solver : public uncopyable
{
    public:

        enum options
        {
            none          = 1,
            force_pangram = 2
        };

    public:

        explicit autogram_solver( int seed )
            : m_table( seed )
        {

        }

    public:

        void set_execution_state( atomic_bool_wrapper *is_running )
        {
            m_execution_state.set( is_running );
        }

        std::string compute( const std::string &sentence,
                             int max_iterations,
                             int result_type )
        {
            std::string result;

            int error             = 0;
            int error_counter     = 0;
            int error_counter_max = 2;

            // Initialize the solution using random values

            error = initialize( sentence, result_type );

            int iterations = 0;

            while ( m_execution_state.is_running() )
            {
                iterations++;

                if ( iterations > max_iterations )
                    break;

                int error_prev = error;

                // Update the solution

                error = update( sentence, result_type );

                if ( error == 0 )
                {
                    // The correct solution has been found

                    result = m_buffer_string.value();

                    m_execution_state.stop();
                }
                else
                {
                    if ( error >= error_prev )
                        error_counter++;

                    if ( error_counter > error_counter_max )
                    {
                        // The algorithm doesn't seem to converge
                        // Initialize the solution using random values

                        error = initialize( sentence, result_type );

                        error_counter = 0;
                    }
                }
            }

            return result;
        }

    private:

        int initialize( const std::string &sentence, int result_type )
        {
            m_buffer_string = "";

            return update( sentence, result_type );
        }

        int update( const std::string &sentence, int result_type )
        {
            int error = 0;

            if ( m_buffer_string.empty() )
            {
                int offset = result_type == autogram_solver::options::force_pangram ? 1 : 0;

                int v_sum = 0;

                while ( v_sum == 0 )
                {
                    m_table.random( offset );

                    v_sum = m_table.sum();
                }

                error = static_cast< int >( m_table.size() * 1000 );
            }
            else
            {
                m_table.backup();
                m_table.update( m_buffer_string.value() );

                error = m_table.error();
            }

            int idx_min = m_table.size() - 1;
            int idx_max = 0;

            for ( int i = 0; i < m_table.size(); ++i )
            {
                int count = m_table.get( i );

                if ( count == 0 )
                    continue;

                idx_min = std::min( idx_min, i );
                idx_max = std::max( idx_max, i );
            }

            m_buffer_string = sentence;

            for ( int i = 0; i < m_table.size(); ++i )
            {
                int count = m_table.get( i );

                if ( count == 0 )
                    continue;

                if ( i == idx_min )
                    m_buffer_string << " ";
                else if ( i == idx_max )
                    m_buffer_string << " and ";
                else
                    m_buffer_string << ", ";

                char c = static_cast< char >( i + static_cast< int >( 'a' ) );

                m_buffer_string << m_number_converter( count ) << " " << c
                                << ( count > 1 ? "'s" : "" );
            }

            return error;
        }

    private:

        buffer_string    m_buffer_string;
        execution_state  m_execution_state;
        number_converter m_number_converter;
        table            m_table;
};
