#include <sstream>
#include "common/jsonUtil.h"
#include "pez/pez.h"
#include "pez/bstrlib.h"

using namespace std;
using namespace glm;

const char*
toString(vec2 v)
{
    static bstring msg;
    bdestroy(msg);
    msg = bformat("[%f, %f]", v.x, v.y);
    return bdata(msg);
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
    while (count--) {
        sstr << '"' << toString(ptrArray[count]) << '"';
        if (count > 0) {
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
