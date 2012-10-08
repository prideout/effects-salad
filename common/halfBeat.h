#pragma once

struct HalfBeat {
    HalfBeat();
    bool Update(bool beat, float time);
    float _previousBeatTime;
    float _currentBeatInterval;
    bool _downBeat;
    float _multiplier;
};
