#include <string>
#include "jsoncpp/json.h"
#include "common/typedefs.h"
#include "glm/glm.hpp"

glm::vec4 vec4FromString(std::string s);
glm::vec2 vec2FromString(std::string s);

const char* toString(Vec3List v);
const char* toString(glm::vec4 v);
const char* toString(glm::vec3 v);
const char* toString(glm::vec2 v);
const char* toString(void** ptrArray, size_t count);
const char* toString(void* ptr);

Json::Value toJson(const char* fmt, ...);
void appendJson(Json::Value& root, const char* fmt, ...);

void ReadJsonFile(std::string filename, Json::Value* root);
