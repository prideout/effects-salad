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
    if (_currentCommand >= _history->size()) {
        return;
    }
    const Json::Value& cmd = _GetCurrentCommand();
    if (percentage == 0) {
        Json::FastWriter writer;
        cout << writer.write(cmd);
    }
    string cmdName = cmd[0u].asString();
    if (cmdName == "AddRectangle") {
        // TODO
    } else if (cmdName == "AddPolygon") { 
        // TODO
    } else if (cmdName == "AddInscribedRectangle") { 
        // TODO
    } else if (cmdName == "AddInscribedPolygon") { 
        // TODO
    } else if (cmdName == "PushPaths") { 
        // TODO
    } else if (cmdName == "PushPath") { 
        // TODO
    }
}

