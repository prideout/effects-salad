#pragma once

#include "common/effect.h"
#include "common/texture.h"
#include "common/timer.h"
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

class FpsOverlay : public Effect {
    Texture2D _numerals;
    Timer _timer;
    GLuint _vbo;
    GLuint _vao;
    float* _WriteGlyphVertex(const Glyph& glyph, glm::vec2 position, int corner, float* vertex);
public:
    FpsOverlay() : Effect() {}
    virtual ~FpsOverlay() {}
    virtual void Init();
    virtual void Update();
    virtual void Draw();
    static const Glyph NumeralGlyphs[];
};
