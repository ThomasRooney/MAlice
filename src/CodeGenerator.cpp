
#include "CodeGenerator.h"

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
        //m_module->dump();
        m_module->print(outputStream, NULL);
        
        return output;
    }
    
}