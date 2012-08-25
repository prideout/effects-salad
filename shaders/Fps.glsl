-- VS

layout(location = 0) in vec2 Position;
layout(location = 2) in vec2 TexCoord;

uniform vec2 InverseViewport;

out vec2 vTexCoord;

void main()
{
    gl_Position = vec4(Position * InverseViewport, 0, 1);
    vTexCoord = TexCoord;
    vTexCoord.y = 1 - vTexCoord.y;
}

-- FS

in vec2 vTexCoord;
out vec4 FragColor;
uniform sampler2D Numerals;

void main()
{
    float x = texture(Numerals, vTexCoord).r;
    FragColor = vec4(1, 1, 1, x);
}
