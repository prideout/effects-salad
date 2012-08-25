#include "fx/fpsOverlay.h"
#include "common/programs.h"
#include "common/init.h"
#include "string.h"

using namespace std;
using namespace glm;

static const int MaxNumDigits = 3;
static const int VertsPerDigit = 6;
static const int FloatsPerVert = 4;
static const int FloatsPerDigit = VertsPerDigit * FloatsPerVert;
static const int TexCoordOffset = sizeof(float) * 2;
static const int BytesPerVert = sizeof(float) * FloatsPerVert;
static const int BytesPerDigit = sizeof(float) * FloatsPerDigit;

const Glyph FpsOverlay::NumeralGlyphs[] = {
    {{ 0, 0 }, { 2, -19, 16, 19, 19, 0 }},
    {{ 19, 0 }, { 1, -20, 14, 20, 16, 0 }},
    {{ 37, 0 }, { 1, -19, 17, 19, 20, 0 }},
    {{ 57, 0 }, { 0, -18, 16, 23, 17, 0 }},
    {{ 77, 0 }, { 0, -19, 21, 24, 22, 0 }},
    {{ 102, 0 }, { 0, -18, 18, 23, 17, 0 }},
    {{ 123, 0 }, { 3, -23, 19, 23, 21, 0 }},
    {{ 146, 0 }, { 2, -18, 18, 23, 18, 0 }},
    {{ 168, 0 }, { 2, -23, 15, 23, 18, 0 }},
    {{ 186, 0 }, { 0, -17, 19, 24, 20, 0 }},
};

void
FpsOverlay::Init()
{
    name = "FpsOverlay";
    Effect::Init();
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("Fps"));
    _numerals.Init("numerals.png");
    _timer.Init();

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    int totalSize = BytesPerDigit * MaxNumDigits;
    glBufferData(GL_ARRAY_BUFFER, totalSize, 0, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(AttrPosition, 2, GL_FLOAT, GL_FALSE, BytesPerVert, 0);
    glEnableVertexAttribArray(AttrPosition);

    glVertexAttribPointer(AttrTexCoord, 2, GL_FLOAT, GL_FALSE, BytesPerVert, offset(TexCoordOffset));
    glEnableVertexAttribArray(AttrTexCoord);
}

void
FpsOverlay::Update()
{
    Effect::Update();
    _timer.Update();
}

float*
FpsOverlay::_WriteGlyphVertex(const Glyph& glyph, vec2 position, int corner, float* vertex)
{
    vec2 textureSize = vec2(_numerals.GetSize());
    vec2 texcoord;
    texcoord.x = glyph.Position.X;
    texcoord.y = glyph.Position.Y + glyph.Metrics.Height;
    
    position.y -= glyph.Metrics.Height + glyph.Metrics.YBearing;
    
    if (corner % 2) {
        position.x += glyph.Metrics.Width;
        texcoord.x += glyph.Metrics.Width;
    }
    
    if (corner / 2) {
        position.y += glyph.Metrics.Height;
        texcoord.y -= glyph.Metrics.Height;
    }
    
    *vertex++ = position.x;
    *vertex++ = position.y;
    *vertex++ = (1 + texcoord.x) / textureSize.x;
    *vertex++ = 1 - (1 + texcoord.y) / textureSize.y;
    
    return vertex;
}

void
FpsOverlay::Draw()
{
    Effect::Draw();
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("Fps"));

    float fps = round(_timer.GetFPS());

    char digits[MaxNumDigits + 1] = {0};
    sprintf(digits, "%d", (int) fps);
    int numDigits = strlen(digits);
    vec2 pos(5, 10);

    vector<float> vbo(numDigits * FloatsPerDigit);
    float* vertex = &vbo[0];
    for (char* digit = &digits[0]; *digit; ++digit) {
        int glyphIndex = *digit - '0';
        const Glyph& glyph = NumeralGlyphs[glyphIndex];
        vertex = _WriteGlyphVertex(glyph, pos, 0, vertex);
        vertex = _WriteGlyphVertex(glyph, pos, 1, vertex);
        vertex = _WriteGlyphVertex(glyph, pos, 2, vertex);
        vertex = _WriteGlyphVertex(glyph, pos, 2, vertex);
        vertex = _WriteGlyphVertex(glyph, pos, 3, vertex);
        vertex = _WriteGlyphVertex(glyph, pos, 1, vertex);
        pos.x += glyph.Metrics.XAdvance;
    }

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, BytesPerDigit * numDigits, &vbo[0]);
    _numerals.Bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLES, 0, numDigits * VertsPerDigit);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
