#pragma once

#include <algorithm>
#include <cctype>
#include <exception>
#include <locale>
#include <string>
#include <thread>

#include "autogram_solver.hpp"

#include "../utils/atomic_bool_wrapper.hpp"
#include "../utils/uncopyable.hpp"

class autogram_solver_helper : public uncopyable
{
    public:

        autogram_solver_helper()
        {

        }

    public:

        std::string compute( const std::string &sentence,
                             int max_iterations,
                             int result_type )
        {
            std::string result;

            std::exception_ptr e_ptr;

            atomic_bool_wrapper is_running( true );

            int processor_count = static_cast< int >( std::thread::hardware_concurrency() );

            // Launch as many solvers as possible

            #pragma omp parallel for
            for ( int i = 0; i < processor_count; ++i )
            {
                std::string autogram;

                try
                {
                    autogram_solver solver( i );

                    solver.set_execution_state( &is_running );

                    autogram = solver.compute( check_sentence( sentence ),
                                               check_max_iterations( max_iterations ),
                                               check_result_type( result_type ) );
                }
                catch ( ... )
                {
                    #pragma omp critical
                    {
                        e_ptr = std::current_exception();
                    }

                    is_running.set_false();
                }

                #pragma omp critical
                {
                    if ( !autogram.empty() )
                        result = autogram;
                }
            }

            if ( e_ptr != nullptr )
                std::rethrow_exception( e_ptr );

            return result;
        }

    private:

        std::string check_sentence( const std::string &sentence )
        {
            std::string sentence_checked;

            sentence_checked = sentence;

            for ( auto &c : sentence_checked )
                c = std::tolower( c, std::locale() );

            return sentence_checked;
        }

        int check_max_iterations( int max_iterations )
        {
            int max_iterations_checked;

            max_iterations_checked = std::max( max_iterations, 1 );

            return max_iterations_checked;
        }

        int check_result_type( int result_type )
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
};
