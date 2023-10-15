#pragma once

#include <algorithm>
#include <atomic>
#include <sstream>
#include <string>

#include "../utils/distribution.hpp"
#include "../utils/execution_state.hpp"
#include "../utils/number_converter.hpp"
#include "../utils/table.hpp"
#include "../utils/uncopyable.hpp"

namespace autogram
{
    enum options
    {
        none          = 1,
        force_pangram = 2
    };
}

class autogram_solver : public uncopyable
{
    public:

        explicit autogram_solver( int seed, std::atomic_bool *is_running = nullptr )
            : m_distribution( seed )
            , m_execution_state( is_running )
        {

        }

    public:

        std::string compute( const std::string &sentence,
                             int max_iterations,
                             int result_type )
        {
            std::string result;

            int error           = 0;
            int error_counter   = 0;
            int error_threshold = 10;

            // Initialize the solution using random values
            std::string autogram = init( sentence, result_type, &error );

            int iterations = 0;

            while ( m_execution_state.is_running() )
            {
                iterations++;

                if ( iterations > max_iterations )
                    break;

                int error_old = error;

                // Update the solution
                autogram = update( sentence, autogram, result_type, &error );

                if ( error == 0 )
                {
                    result = autogram;

                    m_execution_state.stop();
                }
                else
                {
                    if ( error >= error_old )
                        error_counter++;

                    if ( error_counter > error_threshold )
                    {
                        // Initialize the solution using random values
                        autogram = init( sentence, result_type, &error );

                        error_counter = 0;
                    }
                }
            }

            return result;
        }

    private:

        const std::string init( const std::string &sentence,
                                int result_type,
                                int *error )
        {
            return update( sentence, "", result_type, error );
        }

        const std::string update( const std::string &sentence,
                                  const std::string &autogram,
                                  int result_type,
                                  int *error )
        {
            if ( autogram.empty() )
            {
                int offset = result_type == autogram::options::force_pangram ? 1 : 0;

                int v_sum = 0;

                while ( v_sum == 0 )
                {
                    m_table.random( m_distribution, offset );

                    v_sum = m_table.sum();
                }

                if ( error != nullptr )
                    *error = static_cast< int >( m_table.size() * 1000 );
            }
            else
            {
                if ( error != nullptr )
                    m_table.backup();

                m_table.update( autogram );

                if ( error != nullptr )
                    *error = m_table.error();
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

            std::stringstream buf;

            buf << sentence;

            for ( int i = 0; i < m_table.size(); ++i )
            {
                int count = m_table.get( i );

                if ( count == 0 )
                    continue;

                if ( i == idx_min )
                    buf << " ";
                else if ( i == idx_max )
                    buf << " and ";
                else
                    buf << ", ";

                char c = static_cast< char >( i + static_cast< int >( 'a' ) );

                buf << m_number_converter( count ) << " " << c << ( count > 1 ? "'s" : "" );
            }

            return buf.str();
        }

    private:

        distribution m_distribution;

        execution_state m_execution_state;

        number_converter m_number_converter;

        table m_table;
};
