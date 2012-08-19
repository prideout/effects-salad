#include "programs.h"
#include "init.h"

Programs* Programs::_instance(NULL);

Programs::Programs() {
    /* nothing */
}

GLuint 
Programs::operator[](const string& name) {
    // avoid accidentially adding missing entries
    if (_progMap.find(name) == _progMap.end())
        return NULL;
    
    return _progMap[name];
}

GLuint
Programs::Load(const string& name, bool hasGs /*false*/) {
    if (hasGs) {
        return Load(name, name + ".FS", name + ".VS", name + ".GS");
    } else {
        return Load(name, name + ".FS", name + ".VS");
    }
}
 
GLuint
Programs::Load(const string& name, 
                const string& fsName,
                const string& vsName) {

    return _progMap[name] = 
                ::InitProgram(fsName.c_str(), vsName.c_str(), NULL);
}

GLuint
Programs::Load(const string& name, 
                const string& fsName,
                const string& vsName, 
                const string& gsName) {
     
    return _progMap[name] = 
                ::InitProgram(fsName.c_str(), vsName.c_str(), gsName.c_str());
}
