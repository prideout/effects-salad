#include "common/halfBeat.h"

HalfBeat::HalfBeat()
{
    _previousBeatTime = 0;
    _currentBeatInterval = 0;
    _downBeat = false;
    _multiplier = 2.0f;
}

bool HalfBeat::Update(bool beat, float time)
{
    if (beat) {
        _currentBeatInterval = time - _previousBeatTime;
        _previousBeatTime = time;
        _downBeat = false;
        return true;
    }
    if (_downBeat) {
        return false;
    }
    float dt = time - _previousBeatTime;
    if (dt * _multiplier > _currentBeatInterval) {
        _downBeat = true;
        return true;
    }
    return false;
}
