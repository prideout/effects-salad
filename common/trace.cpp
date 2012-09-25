#include "trace.h"
#include <ctime>

TraceScope::TraceScope(std::string scopeName) :
    _scopeName(scopeName)
{
    _start = clock() / float(CLOCKS_PER_SEC);
}


TraceScope::~TraceScope()
{
    // TODO: place this scope in the tree of scopes in Trace
}
