-- Facets.VS

layout(location = 0) in vec4 Position;

uniform mat4 Projection;
uniform mat4 Modelview;

uniform vec3 Translate;
uniform vec3 Scale;

out vec3 vPosition;

void main()
{
    vPosition = Position.xyz * Scale + Translate;
    gl_Position = Projection * Modelview * vec4(vPosition, 1);
}

-- Facets.GS

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float Hue;
uniform mat3 NormalMatrix;
uniform mat4 Projection;
uniform mat4 Modelview;

vec3 HSVtoRGB(vec3 color);

in vec3 vPosition[3];

out vec4 gColor;
out vec3 gFacetNormal;
out vec3 gPosition;

void main()
{
    float hue   = Hue;
    float sat   = 0.3;
    float value = 0.7;
    vec3 hsv = vec3(hue, sat, value);
    gColor =  vec4(HSVtoRGB(hsv), 1.0);

    vec3 A = vPosition[2].xyz - vPosition[0].xyz;
    vec3 B = vPosition[1].xyz - vPosition[0].xyz;
    gFacetNormal = NormalMatrix * normalize(cross(A, B));

    gPosition = vPosition[0];
    gl_Position = Projection * Modelview * vec4(vPosition[0], 1);
    EmitVertex();

    gPosition = vPosition[1];
    gl_Position = Projection * Modelview * vec4(vPosition[1], 1);
    EmitVertex();

    gPosition = vPosition[2];
    gl_Position = Projection * Modelview * vec4(vPosition[2], 1);
    EmitVertex();
    EndPrimitive();
}

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

-- Facets.FS

in vec3 gPosition;
in vec4 gColor;
in vec3 gFacetNormal;
out vec4 FragColor;

uniform vec3 LightPosition = vec3(0, 0, 1);
uniform vec3 AmbientMaterial = vec3(0.1, 0.1, 0.1);

void main()
{
    if (gColor.a == 0) {
        discard;
    }

    vec3 N = normalize(gFacetNormal);
    vec3 L = LightPosition;
    float df = abs(dot(N, L));
    vec3 color = AmbientMaterial + df * gColor.rgb;

    // Craptastic AO:
    float d = clamp((gPosition.y+5)/10, 0, 1);
    color *= d;

    FragColor = vec4(color, gColor.a);
}

-- XZPlane.VS

uniform float Y = 0;
uniform float Scale = 100;

uniform mat4 Projection;
uniform mat4 Modelview;

out vec4 vPosition;

void main()
{
    int i = gl_VertexID;
    vec2 tc = vec2(i%2,i/2);
    vec2 p = 2*tc - 1;
    p *= Scale;
    vPosition = vec4(p.x, Y, p.y, 1);
    gl_Position = Projection * Modelview * vec4(p.x, Y, p.y, 1);
}

-- XZPlane.FS

out vec4 FragColor;

void main()
{
    FragColor = vec4(1);
}

-- XZPlane.CrapAO.FS

in vec4 vPosition;
out vec4 FragColor;

const float r = 10;
const int n = 5;
const float dtheta = 6.28 / float(n);
const float upperBound = 6.28 - dtheta / 2;
 
float distFromShape(vec4 p)
{
    float d = dot(p, vec4(1, 0, 0, r));
    for (float theta = dtheta; theta < upperBound; theta += dtheta) {
        float x = dot(p, vec4(cos(theta), 0, sin(theta), r));
        d = min(d, x);
    }
    return d;
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

int nSamples = 5;

void main()
{
    float d1 = distFromShape(vPosition);
    float d2 = 0;
    for (uint i = 0u; i < uint(nSamples); i++) {
        vec2 e = vec2(randhash(i*2u,2), randhash(i*2u+1u,2)) - vec2(1);
        d2 += distFromShape(vPosition + vec4(e.x, 0, e.y, 0));
    }
    float d = d2 - d1;

    d = 1 - clamp(d/60, 0, 1);

    // Distance to box:
    //   http://www.iquilezles.org/blog/?p=1154

    FragColor = vec4(d * vec3(0.8), 1);
}

-- Terrain.VS

layout(location = 0) in vec4 Position;
layout(location = 1) in vec3 Normal;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat3 NormalMatrix;

out vec3 vPosition;
out vec3 vNormal;

void main()
{
    vPosition = Position.xyz;
    vNormal = NormalMatrix * Normal;
    gl_Position = Projection * Modelview * Position;
}

-- Terrain.FS

in vec3 vPosition;
in vec3 vNormal;

uniform vec3 LightPosition = vec3(0, 0, 1);
uniform vec3 AmbientMaterial = vec3(0.1, 0.1, 0.1);
uniform vec3 Color = vec3(1);

out vec4 FragColor;

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = LightPosition;
    float df = abs(dot(N, L));
    vec3 C = AmbientMaterial + df * Color;
    FragColor = vec4(C, 1);
}
