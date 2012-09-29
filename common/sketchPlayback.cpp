#include "common/jsonUtil.h"
#include "common/sketchPlayback.h"
#include "common/demoContext.h"

using namespace sketch;
using namespace std;

Playback::Playback(const Json::Value& history, sketch::Scene* scene) :
    _history(&history),
    _scene(scene),
    _commandDuration(1.0),
    _currentCommand(0),
    _currentCommandStartTime(-1),
    _previousTime(0)
{
    cout << _history->size() << " commands in sketchPlayback.\n";
}

void
Playback::SetCommandDuration(float seconds)
{
    _commandDuration = seconds;
}

const Json::Value&
Playback::_GetCurrentCommand() const
{
    return (*_history)[_currentCommand];
}

bool
Playback::_IsDiscreteCommand(const Json::Value& cmd) const
{
    string cmdName = cmd[0u].asString();
    string addPrefix("Add");
    return !cmdName.compare(0, addPrefix.size(), addPrefix);
}

void
Playback::Update()
{
    float time = DemoContext::totalTime;
    if (time == _previousTime) {
        return;
    }
    _previousTime = time;

    // Special case the first command so that it always gets executed.
    if (_currentCommandStartTime < 0) {
        _currentCommandStartTime = time;
        _currentCommand = 0;

    // Bump to the next command if it's time.
    } else if (_IsDiscreteCommand(_GetCurrentCommand())) {
        _currentCommandStartTime = time;
        ++_currentCommand;
    } else if ((time - _currentCommandStartTime) > _commandDuration) { 
        _ExecuteCurrentCommand(1.0);
        _currentCommandStartTime = time;
        ++_currentCommand;
    }

    // Execute the command
    float percentage = (time - _currentCommandStartTime) / _commandDuration;
    _ExecuteCurrentCommand(percentage);
}

void
Playback::_ExecuteCurrentCommand(float percentage)
{
    #if 0
    if (_currentCommand >= _history->size()) {
        return;
    }
    const Json::Value& cmd = _GetCurrentCommand();
    if (percentage == 0) {
        Json::FastWriter writer;
        cout << writer.write(cmd);
    } else {
        return; // TODO this it temporary until we start honoring 'percentage'
    }
    string cmdName = cmd[0u].asString();
    if (cmdName == "AddRectangle") {
        string handle = cmd[1u].asString();
        float width = cmd[2u].asDouble();
        float height = cmd[3u].asDouble();
        vec4 eqn = vec4FromString(cmd[4u].asString());
        vec2 offset = vec2FromString(cmd[5u].asString());
        CoplanarPath* path = _scene->AddRectangle(width, height, eqn, offset);
        _handles[handle] = path;
    } else if (cmdName == "AddPolygon") { 
        string handle = cmd[1u].asString();
        float radius = cmd[2u].asDouble();
        vec4 eqn = vec4FromString(cmd[3u].asString());
        vec2 offset = vec2FromString(cmd[4u].asString());
        int numPoints = cmd[5u].asInt();
        CoplanarPath* path = _scene->AddPolygon(radius, eqn, offset, numPoints);
        _handles[handle] = path;
    } else if (cmdName == "AddInscribedRectangle") { 
        string handle = cmd[1u].asString();
        float width = cmd[2u].asDouble();
        float height = cmd[3u].asDouble();
        Path* outer = _handles[cmd[4u].asString()];
        CoplanarPath* cop = dynamic_cast<CoplanarPath*>(outer);
        vec2 offset = vec2FromString(cmd[5u].asString());
        CoplanarPath* path = _scene->AddInscribedRectangle(width, height, cop, offset);
        _handles[handle] = path;
    } else if (cmdName == "AddInscribedPolygon") { 
        string handle = cmd[1u].asString();
        float radius = cmd[2u].asDouble();
        Path* outer = _handles[cmd[3u].asString()];
        CoplanarPath* cop = dynamic_cast<CoplanarPath*>(outer);
        vec2 offset = vec2FromString(cmd[4u].asString());
        int numPoints = cmd[5u].asInt();
        CoplanarPath* path = _scene->AddInscribedPolygon(radius, cop, offset, numPoints);
        _handles[handle] = path;
    } else if (cmdName == "PushPaths") { 
        // TODO
    } else if (cmdName == "PushPath") { 
        // TODO
    }
    #endif
}

