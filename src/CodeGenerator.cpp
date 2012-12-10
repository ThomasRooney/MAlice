
#include "CodeGenerator.h"

#include <sstream>
#include "llvm/Support/raw_ostream.h"

namespace MAlice {
    
    CodeGenerator::CodeGenerator(llvm::Module *module)
    {
        m_module = module;
    }
    
    std::string CodeGenerator::generateCode()
    {
        std::string output;
        if (!m_module)
            return "";
        
        llvm::raw_string_ostream outputStream(output);
        #ifdef __APPLE__
        m_module->dump();
        #else
        m_module->print(outputStream, NULL);
        #endif
        //outputStream. >> output;
        return output;
    }
    
}