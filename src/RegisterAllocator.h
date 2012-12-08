#ifndef _REGISTER_ALLOCATOR
#define _REGISTER_ALLOCATOR

#include "LLVMHeader.h"

namespace MAlice {

    class RegisterAllocator {
    private:
        int numRequested;
    public:
        RegisterAllocator ();
        int RequestNewRegister(llvm::Module *);
    }; 

} // namespace MALice

#endif //_REGISTER_ALLOCATOR