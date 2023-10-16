// Per la risoluzione degli autogrammi viene proposto un algoritmo di minimizzazione.
//
// Passi dell'algoritmo:
//     - Si parte da una soluzione casuale.
//     - Si raffina la soluzione iterativamente.
//     - Se il processo non converge, si torna al passo iniziale.
//
// L'algoritmo termina quando:
//     - Un processo di minimizzazione converge al risultato corretto.
//     - Viene raggiunto il numero massimo di iterazioni specificato.
//
// La classe "autogram_solver" implementa l'algoritmo appena descritto.
//
// La classe "autogram_solver_helper" esegue parallelamente piu' istanze dell'algoritmo.
//
// Utilizzo da linea di comando:
//     - autograms.exe [sentence] [-i max iterations] [-o options]
//
// Se l'algoritmo non trova alcuna soluzione valida, si puo' provare ad aumentare il
// numero massimo di iterazioni (default 500000) con l'apposito parametro.
//
// Si puo' forzare la ricerca di un autogramma che sia anche un pangramma con il
// parametro "-o force-pangram".
//
// Esempi di utilizzo:
//     - autograms.exe "this sentence contains"
//     - autograms.exe "this sentence contains" -i 1000000
//     - autograms.exe "this sentence contains" -o force-pangram
//     - autograms.exe "this sentence contains" -i 1000000 -o force-pangram

#if defined( _DEBUG )
#include <crtdbg.h>
#endif

#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "core/autogram_solver_helper.hpp"
#include "utils/clock.hpp"

int main( int argc, char **argv )
{
    #if defined( _DEBUG )
    _CrtSetDbgFlag( _CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF );
    // _CrtSetBreakAlloc( 0 );
    #endif

    try
    {
        std::string sentence = "this sentence contains";

        int max_iterations = 500000;

        int result_type = autogram::options::none;

        if ( argc > 1 )
        {
            sentence = std::string( argv[ 1 ] );

            if ( ( argc - 2 ) % 2 == 0 )
            {
                for ( int i = 0; i < ( argc - 2 ) / 2; ++i )
                {
                    int idx1 = i * 2 + 2;
                    int idx2 = i * 2 + 3;

                    if ( std::string( argv[ idx1 ] ) == "-i" )
                        max_iterations = std::stoi( argv[ idx2 ] );

                    if ( std::string( argv[ idx1 ] ) == "-o" )
                        result_type = std::string( argv[ idx2 ] ) == "force-pangram"
                                      ? autogram::options::force_pangram
                                      : autogram::options::none;
                }
            }
        }

        clock::tic();

        std::cout << "Computing..." << std::endl;

        autogram_solver_helper solver;

        std::string result = solver.compute( sentence, max_iterations, result_type );

        if ( result.empty() )
        {
            std::cout << "No valid solution found." << std::endl;
        }
        else
        {
            std::cout << "The solution found is \"" << result << "\"." << std::endl;

            std::stringstream buf;

            buf << std::setiosflags( std::stringstream::fixed )
                << std::setiosflags( std::stringstream::showpoint )
                << std::setprecision( 4 );

            buf << "Done in " << clock::toc() << " seconds.";

            std::cout << buf.str() << std::endl;
        }
    }
    catch ( std::exception &e )
    {
        std::cout << "+++ ERROR +++ (" << e.what() << ")" << std::endl;
    }

    return 0;
}
