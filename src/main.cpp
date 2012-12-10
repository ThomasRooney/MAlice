#include "CompilerFrontEnd.h"

#include <cstdlib>

int main(int argc, char *argv[])
{
    MAlice::CompilerFrontEnd frontEnd = MAlice::CompilerFrontEnd(argc, argv);
    
    if (frontEnd.run())
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;

}