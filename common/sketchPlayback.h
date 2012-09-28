#pragma once
#include "common/sketchScene.h"
#include "common/typedefs.h"

namespace sketch
{
    class Playback
    {
    public:
        Playback(const Json::Value& history, sketch::Scene* scene);
        void SetCommandDuration(float seconds);
        void Update();
    private:
        void _ExecuteCurrentCommand(float percentage);
        const Json::Value& _GetCurrentCommand() const;
        bool _IsDiscreteCommand(const Json::Value&) const;
        const Json::Value* _history;
        sketch::Scene* _scene;
        float _commandDuration;
        unsigned _currentCommand;
        float _currentCommandStartTime;
        float _previousTime;
    };
}
