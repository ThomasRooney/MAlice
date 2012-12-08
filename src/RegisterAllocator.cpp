#include "RegisterAllocator.h"

namespace MAlice {

    RegisterAllocator::RegisterAllocator ()
    {
        numRequested = 0 ;
    }

    int RegisterAllocator::RequestNewRegister(llvm::Module *)
    {
        return numRequested++;
    }
}