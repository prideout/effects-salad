#pragma once

#include "pez/pez.h"

#include <string>
#include <map>

using namespace std;

//
// A helper class for managing shader programs
//
class Programs {
    typedef map<string,GLuint> _ProgMap;
    
    static Programs* _instance;
    _ProgMap _progMap;

    // Private default constructor; singleton
    Programs();

public:
    

    // Singleton pattern
    static Programs&
    GetInstance() 
    {
        if (not _instance) {
            _instance = new Programs();
        }

        return *_instance;
    }

    GLuint operator[](const string& name);
    
    GLuint Load(const string& name, bool hasGs = false);

    GLuint Load(const string& name,
                const string& fsName,
                const string& vsName);

    GLuint Load(const string& name,
                const string& fsName,
                const string& vsName,
                const string& gsName);

};
