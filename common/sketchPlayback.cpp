#include "common/sketchPlayback.h"
#include "common/demoContext.h"

using namespace sketch;
using namespace std;

Playback::Playback(const Json::Value& history, sketch::Scene* scene) :
    _history(&history),
    _scene(scene),
    _commandDuration(2.0),
    _currentCommand(999)
{
    cout << _history->size() << " commands in sketchPlayback.\n";
    //Json::StyledWriter writer;
    //cout << writer.write(*_history) << endl;
}

void
Playback::SetCommandDuration(float seconds)
{
    _commandDuration = seconds;
}

void
Playback::Update()
{
    float elapsed = DemoContext::totalTime;

    float cmdStartTime = 0;
    float cmdEndTime = _commandDuration;
    for (unsigned i = 0; i < _history->size(); ++i) {

        const Json::Value& node = (*_history)[i];
        string cmd = node[0u].asString();

        std::string push("Push");
        bool isPushCmd = !cmd.compare(0, push.size(), push);

        if (cmdStartTime <= elapsed && elapsed < cmdEndTime) {
            if (i != _currentCommand) {
                _currentCommand = i;
                //cout << cmd << endl;
                // TODO process it here...
                if (!isPushCmd) {
                    continue;
                }
            }
            break;
        }

        if (isPushCmd) {
            cmdStartTime = cmdEndTime;
            cmdEndTime += _commandDuration;
        }
    }
}
