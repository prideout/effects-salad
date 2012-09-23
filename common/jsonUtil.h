#include <string>
#include "jsoncpp/json.h"
#include "glm/glm.hpp"

const char* toString(glm::vec2 v);
const char* toString(void** ptrArray, size_t count);
const char* toString(void* ptr);

Json::Value toJson(const char* fmt, ...);
void appendJson(Json::Value& root, const char* fmt, ...);
