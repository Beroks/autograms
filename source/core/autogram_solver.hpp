#pragma once

#include <algorithm>
#include <cctype>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../utils/atomic_bool_wrapper.hpp"
#include "../utils/buffer_string.hpp"
#include "../utils/distribution.hpp"
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
            : m_distribution( seed )
        {

        }

    public:

        std::string compute( const std::string &sentence,
                             int max_iterations,
                             int result_type )
        {
            // Clean up the parameters and start the solver

            return run( check_sentence( sentence ),
                        check_max_iterations( max_iterations ),
                        check_result_type( result_type ) );
        }

        void set_execution_state( atomic_bool_wrapper *is_running )
        {
            m_execution_state.set( is_running );
        }

    private:

        std::string check_sentence( const std::string &sentence ) const
        {
            std::string sentence_checked;

            std::stringstream buf( sentence );

            buf >> sentence_checked;

            std::string token;

            while ( buf >> token )
                sentence_checked += " " + token;

            for ( auto &c : sentence_checked )
                c = std::tolower( c, std::locale() );

            if ( sentence_checked.empty() )
                throw std::runtime_error( "autogram_solver : invalid input" );

            return sentence_checked;
        }

        int check_max_iterations( int max_iterations ) const
        {
            int max_iterations_checked;

            max_iterations_checked = std::max( max_iterations, 0 );

            if ( max_iterations_checked == 0 )
                throw std::runtime_error( "autogram_solver : invalid input" );

            return max_iterations_checked;
        }

        int check_result_type( int result_type ) const
        {
            int result_type_checked;

            switch ( result_type )
            {
                case autogram_solver::options::force_pangram:
                    result_type_checked = autogram_solver::options::force_pangram;
                    break;

                default:
                    result_type_checked = autogram_solver::options::none;
                    break;
            }

            return result_type_checked;
        }

    private:

        std::string run( const std::string &sentence,
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
                int v_sum = 0;

                while ( v_sum == 0 )
                {
                    if ( result_type == autogram_solver::options::force_pangram )
                    {
                        auto f = [ &d = m_distribution ]()
                        {
                            int r = d( 50 ) + 1;

                            return r;
                        };

                        m_table.random( f );
                    }
                    else
                    {
                        auto f = [ &d = m_distribution ]()
                        {
                            int r = std::max( 0, d( 100 ) - 80 );

                            return r == 1 ? 0 : r;
                        };

                        m_table.random( f );
                    }

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

                m_buffer_string << m_number_converter( count )
                                << " " << c << ( count > 1 ? "'s" : "" );
            }

            return error;
        }

    private:

        buffer_string    m_buffer_string;
        distribution     m_distribution;
        execution_state  m_execution_state;
        number_converter m_number_converter;
        table            m_table;
};
