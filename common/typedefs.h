#pragma once

#include <vector>
#include "glm/glm.hpp" 

class Effect;

typedef std::vector<unsigned char> Blob;

typedef std::vector<glm::vec3> Vec3List;
typedef std::vector<glm::vec4> Vec4List;
typedef std::vector<unsigned int> IndexList;
typedef std::vector<Effect*> EffectList;

typedef std::vector<float> FloatList;

typedef unsigned int VertexAttribMask;

#define FOR_EACH(var, container)                                \
  for(auto var = (container).begin();    \
      var != (container).end();                                 \
      ++var)

