#include "glm/gtc/type_ptr.hpp"
#include "fx/fpsOverlay.h"
#include "common/programs.h"
#include "common/init.h"
#include "common/demoContext.h"
#include <sstream>
#include <sys/time.h>

using namespace std;
using namespace glm;

static const unsigned MaxNumDigits = 3;
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

static unsigned int
_GetMilliseconds()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000000000 + tp.tv_usec;
}

void
FpsOverlay::Init()
{
    _filterConstant = 0.01;
    _previousTime = 0;
    name = "FpsOverlay";
    Effect::Init();
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("Fps"));
    _numerals.Init("numerals.png");

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
    glUseProgram(progs["Fps"]);

    vec2 viewport = vec2(this->context->viewport.width,
                         this->context->viewport.height);

    vec2 invViewport = 2.0f / viewport;
    glUniform2fv(u("InverseViewport"), 1, ptr(invViewport));
    glUniform1i(u("Numerals"), 0);

    unsigned int time = _GetMilliseconds();
    float deltaTime = (time - _previousTime) / 1000000.0f;
    _previousTime = time;

    double fps = 1.0f / deltaTime;
    double alpha = _filterConstant;
    _fps = fps * alpha + _fps * (1.0 - alpha);

    std::stringstream ssdigits;
    std::string digits;
    ssdigits << int(round(_fps));
    digits = ssdigits.str();
    if (digits.size() > MaxNumDigits) {
        digits = "999";
        // the kalman/decay filter gets "whacked out" if the value is too large
        // so reset the filter memory
        _fps = 0;
    }
    int numDigits = digits.size();
    vec2 pos(-viewport.x / 2 + 5, -viewport.y / 2 + 10);

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
    pezCheckGL("Draw FPS");
}
