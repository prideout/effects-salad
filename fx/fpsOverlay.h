#pragma once

#include "common/effect.h"
#include "common/texture.h"

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
public:
    FpsOverlay() : Effect() {}
    virtual ~FpsOverlay() {}
    virtual void Init();
    virtual void Update();
    virtual void Draw();
    static const Glyph NumeralGlyphs[];
};
