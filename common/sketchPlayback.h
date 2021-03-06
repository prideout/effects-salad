#pragma once
#include "common/jsonUtil.h"
#include "common/typedefs.h"
#include "tween/CppTweener.h"
#include <string>
#include <map>

namespace sketch
{
    struct Path;
    typedef std::map<std::string, sketch::Path*> PathHandles;
    class Tessellator;
    class Scene;

    class Playback
    {
    public:
        Playback(const Json::Value& history,
                 sketch::Scene* scene,
                 sketch::Tessellator* tess);
        void SetCommandDuration(float seconds);
        void Update(bool explicitBump = false, bool bump = false);
    private:
        float _Tween(float goalValue, float percentage);
        void _ExecuteCurrentCommand(float percentage);
        const Json::Value& _GetCurrentCommand() const;
        bool _IsDiscreteCommand(const Json::Value&) const;
    private:
        tween::Tweener _tween;
        const Json::Value* _history;
        sketch::Scene* _scene;
        float _commandDuration;
        unsigned _currentCommand;
        float _currentCommandStartTime;
        float _previousTime;
        PathHandles _handles;
        FloatList _originalPlanes;
        sketch::Tessellator* _tess;
    };
}
