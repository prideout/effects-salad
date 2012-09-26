#pragma once

#include "common/effect.h"
#include "common/texture.h"
#include "glm/glm.hpp"

struct GlyphPosition {
    int X;
    int Y;
};

struct GlyphMetrics {
    int XBearing;
    int YBearing;
    int Width;
    int Height;
    int XAdvance;
    int YAdvance;
};

struct Glyph {
    GlyphPosition Position;
    GlyphMetrics Metrics;
};

// TODO rename to InfoOverlay
class FpsOverlay : public Effect {
public:

    enum Mode {
        FrameRate,
        MemUsage,
    };

    FpsOverlay(Mode m = FrameRate) : Effect(), _mode(m) {}
    virtual ~FpsOverlay() {}
    virtual void Init();
    virtual void Update();
    virtual void Draw();
    static const Glyph NumeralGlyphs[];

private:
    Mode _mode;
    Texture2D _numerals;
    unsigned int _previousTime;
    GLuint _vbo;
    GLuint _vao;
    float* _WriteGlyphVertex(const Glyph& glyph, glm::vec2 position, int corner, float* vertex);
    float _filterConstant;
    float _fps;
};
