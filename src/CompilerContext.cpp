
#include "CompilerContext.h"
#include "SymbolTable.h"

#include <unordered_map>

using namespace std;

namespace MAlice {
    
    CompilerContext::CompilerContext(std::string input)
    {
        m_input = input;
        m_symbolTables.push_back(new SymbolTable());
        t_symbolTable = new SymbolTable();
        configureKeywords();
        #ifdef _WIN32
           temporarySymbolTableLock = CreateMutex(0, FALSE, 0);
        #else
           pthread_mutex_init (&temporarySymbolTableLock, NULL);;
        #endif
    }
    // TODO: Delete SymbolTables in destructor

    bool CompilerContext::lockTemporarySymbolTable()
    {
        #ifdef _WIN32
            return (WaitForSingleObject(temporarySymbolTableLock, INFINITE)==WAIT_FAILED?1:0);  
        #else
           return pthread_mutex_lock( &temporarySymbolTableLock );  
        #endif
    }

    bool CompilerContext::unlockTemporarySymbolTable()
    {
        #ifdef _WIN32
            return (ReleaseMutex(temporarySymbolTableLock)==0);
        #else
            return pthread_mutex_unlock( &temporarySymbolTableLock );
        #endif
    }

    SymbolTable* CompilerContext::getTemporarySymbolTable()
    {
      return t_symbolTable;
    }

    bool CompilerContext::addEntityInScope(std::string identifier, Entity *entity)
    {
        if (m_symbolTables.empty())
            return false;
        
        SymbolTable* & innermostSymbolTable = m_symbolTables.back();
        if (innermostSymbolTable->contains(identifier))
            return false;
        
        innermostSymbolTable->insert(identifier, entity);
        
        return true;
    }
    
    bool CompilerContext::isSymbolInScope(std::string identifier, Entity **outEntity)
    {
        if (m_symbolTables.empty())
            return false;
        
        for (list<SymbolTable*>::reverse_iterator iterator = m_symbolTables.rbegin(); iterator != m_symbolTables.rend(); iterator++) {
            SymbolTable *table = *iterator;
            Entity *entity = table->get(identifier);
            
            if (entity) {
                if (outEntity)
                    *outEntity = entity;
                
                return true;
            }
        }
        
        return false;
     }
    
    bool CompilerContext::isSymbolInCurrentScope(std::string identifier, Entity **outEntity)
    {
        if (m_symbolTables.empty())
            return false;
        
        SymbolTable *table = m_symbolTables.back();
        Entity *entity = table->get(identifier);
        
        return entity != NULL;
    }

    void CompilerContext::enterScope()
    {
        m_symbolTables.push_back(new SymbolTable());
    }
    
    void CompilerContext::exitScope()
    {
        // TODO: generate error if we pop too far.
        if (m_symbolTables.empty())
            return;
        
        SymbolTable* & lastSymbolTable = m_symbolTables.back();
        delete lastSymbolTable;
        
        m_symbolTables.pop_back();
    }
    
    ErrorReporter *CompilerContext::getErrorReporter()
    {
        return m_errorReporter;
    }
    
    void CompilerContext::setErrorReporter(ErrorReporter *errorReporter)
    {
        if (m_errorReporter != NULL)
            m_errorReporter->setInput("");
        
        m_errorReporter = errorReporter;
        m_errorReporter->setInput(m_input);
    }
    
    void CompilerContext::configureKeywords()
    {
        m_keywords.insert("opened");
        m_keywords.insert("closed");
        m_keywords.insert("spider");
        m_keywords.insert("but");
        m_keywords.insert("The");
        m_keywords.insert("looking-glass");
        m_keywords.insert("room");
        m_keywords.insert("spoke");
        m_keywords.insert("was");
        m_keywords.insert("a");
        m_keywords.insert("became");
        m_keywords.insert("Alice");
        m_keywords.insert("found");
        m_keywords.insert("because");
        m_keywords.insert("said");
        m_keywords.insert("then");
        m_keywords.insert("and");
        m_keywords.insert("contained");
        m_keywords.insert("eventually");
        m_keywords.insert("enough");
        m_keywords.insert("times");
        m_keywords.insert("unsure");
        m_keywords.insert("which");
        m_keywords.insert("or");
        m_keywords.insert("so");
        m_keywords.insert("piece");
        m_keywords.insert("what");
        m_keywords.insert("of");
        m_keywords.insert("too");
        m_keywords.insert("maybe");
        m_keywords.insert("perhaps");
        m_keywords.insert("either");
        m_keywords.insert("had");
        m_keywords.insert("ate");
        m_keywords.insert("drank");
        m_keywords.insert("number");
        m_keywords.insert("letter");
        m_keywords.insert("sentence");
    }
    
    bool CompilerContext::isKeyword(std::string string)
    {
        return m_keywords.find(string) != m_keywords.end();
    }
    
}; // namespace MAlice