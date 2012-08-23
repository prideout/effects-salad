#include "timer.h"
#include <iostream>

void
Timer::Init() {
    _start = clock();
}

void
Timer::Draw() {
    if (not visible) return;

    if (drawText and _frameCount == 0) {
        std::cout << "FPS";
        if (name.size())
            std::cout << "[" << name << "]";
        std::cout << ": " << _fps << std::endl;
    }
}

void
Timer::Update() {
    _frameCount++;

    if (_frameCount == MAX_FRAMES) {
        _seconds = float(clock() - _start) / CLOCKS_PER_SEC;
        _fps = _frameCount / _seconds;
        if (_fps > 0)
            _seconds = 1.0 / _fps;
        _frameCount = 0;
        _start = clock();
    }
}

