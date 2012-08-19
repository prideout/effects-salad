#pragma once

#include "glm/glm.hpp"
#include "pez/pez.h"


class DemoContext {
    static const DemoContext* _current;

public:

    static const DemoContext*
    GetCurrent() { 
        return _current; 
    }

    static void
    SetCurrent(const DemoContext* const cur) {
        _current = cur;
    }

};

