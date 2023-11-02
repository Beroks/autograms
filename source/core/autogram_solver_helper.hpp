#pragma once

#include <exception>
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

            int number_of_threads = static_cast< int >( std::thread::hardware_concurrency() );

            // Launch as many solvers as possible

            #pragma omp parallel for
            for ( int i = 0; i < number_of_threads; ++i )
            {
                std::string autogram;

                try
                {
                    autogram_solver solver( i );

                    solver.set_execution_state( &is_running );

                    autogram = solver.compute( sentence, max_iterations, result_type );
                }
                catch ( ... )
                {
                    #pragma omp critical
                    {
                        if ( e_ptr == nullptr )
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
};
