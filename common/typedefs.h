#pragma once

#include <vector>
#include <iostream>
#include "glm/glm.hpp" 

class Effect;

typedef std::vector<unsigned char> Blob;

typedef std::vector<glm::ivec3> TriList;
typedef std::vector<glm::vec3> Vec3List;
typedef std::vector<glm::vec4> Vec4List;
typedef std::vector<unsigned int> IndexList;
typedef std::vector<Effect*> EffectList;

typedef std::vector<float> FloatList;

typedef unsigned int VertexAttribMask;

#define FOR_EACH(var, container)                                \
  for(typeof((container).begin()) var = (container).begin();    \
      var != (container).end();                                 \
      ++var)


// helper function for printing vec3s
inline void
_pvec3(glm::vec3 v) {
    std::cout << "vec3(" << 
        v.x << ", " <<
        v.y << ", " <<
        v.z << ") " << std::endl;
}
