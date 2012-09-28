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
        const Json::Value* _history;
        sketch::Scene* _scene;
        float _commandDuration;
        unsigned _currentCommand;
    };
}
