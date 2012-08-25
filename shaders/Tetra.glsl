-- Simple.VS

layout(location = 0) in vec4 Position;

uniform mat4 Projection;
uniform mat4 Modelview;

void main()
{
    gl_Position = Projection * Modelview * Position;
}

-- Simple.FS

out vec4 FragColor;

void main()
{
    FragColor = vec4(0, 0, 0, 0.75);
}

-- Solid.VS

layout(location = 0) in vec4 Position;
layout(location = 1) in vec3 Normal;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat3 NormalMatrix;
uniform sampler2DRect CentroidTexture;
uniform float CullY;

out vec4 vColor;
out vec3 vFacetNormal;

vec3 HSVtoRGB(vec3 color)
{
    float f, p, q, t, hueRound;
    int hueIndex;
    float hue, saturation, value;
    vec3 result;

    hue = color.r;
    float s = saturation = color.g;
    float v = value = color.b;

    hueRound = floor(hue * 6.0);
    hueIndex = int(hueRound) % 6;
    f = (hue * 6.0) - hueRound;
    p = value * (1.0 - saturation);
    q = value * (1.0 - f*saturation);
    t = value * (1.0 - (1.0 - s)*saturation);

    switch(hueIndex) {
        case 0: result = vec3(v,t,p); break;
        case 1: result = vec3(q,v,p); break;
        case 2: result = vec3(p,v,t); break;
        case 3: result = vec3(p,q,v); break;
        case 4: result = vec3(t,p,v); break;
        case 5: result = vec3(v,p,q); break;
    }
    return result;
}

float randhash(uint seed, float b)
{
    const float InverseMaxInt = 1.0 / 4294967295.0;
    uint i=(seed^12345391u)*2654435769u;
    i^=(i<<6u)^(i>>26u);
    i*=2654435769u;
    i+=(i<<5u)^(i>>12u);
    return float(b * i) * InverseMaxInt;
}

void main()
{
    uint tetid = uint(gl_VertexID) / 12u;
    ivec2 coord = ivec2(int(tetid) % 1024, int(tetid) / 1024);
    float hue = randhash(tetid, 1.0);
    vec3 hsv = vec3(hue, 0.75, 0.75);
    vColor = vec4(HSVtoRGB(hsv), 1.0);

    vec3 tetcenter = texelFetch(CentroidTexture, coord).rgb;
    //tetcenter = Position.xyz;
    if (tetcenter.y > CullY) {
        vColor.a = 0;
    } else {
        vFacetNormal = NormalMatrix * Normal;
    }

    gl_Position = Projection * Modelview * Position;
}

-- Solid.FS

in vec4 vColor;
in vec3 vFacetNormal;
out vec4 FragColor;

uniform vec3 LightPosition = vec3(0, 0, 1);
uniform vec3 AmbientMaterial = vec3(0.1, 0.1, 0.1);

void main()
{
    if (vColor.a == 0) {
        discard;
    }

    vec3 N = normalize(vFacetNormal);
    vec3 L = LightPosition;
    float df = abs(dot(N, L));
    vec3 color = AmbientMaterial + df * vColor.rgb;

    FragColor = vec4(color, vColor.a);
}
