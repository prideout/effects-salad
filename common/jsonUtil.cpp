#include <fstream>
#include <sstream>
#include "common/jsonUtil.h"
#include "pez/pez.h"
#include "pez/bstrlib.h"

using namespace std;
using namespace glm;

// glm/gtx/string_cast.inl can already stringify vectors, but we'd prefer something more JSON-amenable

// I almost feel like inheriting from Json::Value and providing asVec2 etc.. methods for glm.

vec2
vec2FromString(string s)
{
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(s.c_str(), root);
    pezCheck(parsingSuccessful);
    return vec2(root[0u].asDouble(), root[1u].asDouble());
}

vec4
vec4FromString(string s)
{
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(s.c_str(), root);
    pezCheck(parsingSuccessful);
    return vec4(root[0u].asDouble(),
                root[1u].asDouble(),
                root[2u].asDouble(),
                root[3u].asDouble());
}

const char*
toString(vec2 v)
{
    static bstring msg;
    bdestroy(msg);
    msg = bformat("[%f, %f]", v.x, v.y);
    return bdata(msg);
}

const char*
toString(vec3 v)
{
    static bstring msg;
    bdestroy(msg);
    msg = bformat("[%f, %f, %f]", v.x, v.y, v.z);
    return bdata(msg);
}

const char*
toString(vec4 v)
{
    static bstring msg;
    bdestroy(msg);
    msg = bformat("[%f, %f, %f, %f]", v.x, v.y, v.z, v.w);
    return bdata(msg);
}

const char*
toString(Vec3List v)
{
    static string msg;
    ostringstream sstr;
    sstr << '[';
    for (size_t i = 0; i < v.size(); ++i) {
        sstr << '"' << toString(v[i]) << '"';
        if (i < v.size() - 1) {
            sstr << ',';
        }
    }
    sstr << ']';
    msg = sstr.str();
    return msg.c_str();
}

const char*
toString(void* ptr)
{
    static bstring msg;
    bdestroy(msg);
    msg = bformat("%8.8x", ptr);
    return bdata(msg);
}

const char*
toString(void** ptrArray, size_t count)
{
    static string msg;
    ostringstream sstr;
    sstr << '[';
    for (size_t i = 0; i < count; ++i) {
        sstr << '"' << toString(ptrArray[i]) << '"';
        if (i < count - 1) {
            sstr << ',';
        }
    }
    sstr << ']';
    msg = sstr.str();
    return msg.c_str();
}

Json::Value
toJson(const char* pStr, ...)
{
    va_list a;
    va_start(a, pStr);

    static char msg[2048] = {0};
    vsnprintf(msg, countof(msg), pStr, a);

    Json::Value root;
    Json::Reader reader;
    bool success = reader.parse(msg, root);
    pezCheck(success, "Bad JSON: %s", msg); 
    return root;
}

void
appendJson(Json::Value& root, const char* pStr, ...)
{
    va_list a;
    va_start(a, pStr);

    static char msg[2048] = {0};
    vsnprintf(msg, countof(msg), pStr, a);

    Json::Value node;
    Json::Reader reader;
    bool success = reader.parse(msg, node);
    pezCheck(success, "Bad JSON: %s", msg); 
    root.append(node);
}

void ReadJsonFile(string filename, Json::Value* root)
{
    ifstream jsonFile(filename.c_str());
    string jsonString((istreambuf_iterator<char>(jsonFile)),
                      istreambuf_iterator<char>());
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonString.c_str(), *root);
    if (!parsingSuccessful) {
        cerr  << "Failed to parse JSON file: "
              << filename << endl
              << reader.getFormatedErrorMessages();
        exit(1);
    }
}
