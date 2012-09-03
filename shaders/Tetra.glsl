-- Cracks.VS

layout(location = 0) in vec4 Position;
layout(location = 4) in float Length;

uniform vec3 Translate;
uniform vec3 Scale;

uniform mat4 Projection;
uniform mat4 Modelview;

out float vLength;
out vec3 vPosition;

void main()
{
    vLength = Length;
    vPosition = Position.xyz * Scale + Translate;
    gl_Position = Projection * Modelview * vec4(vPosition, 1);
}

-- Cracks.FS

in float vLength;
out vec4 FragColor;
uniform vec4 Color = vec4(0, 0, 0, 0.75);
uniform float Time;
uniform float DepthOffset;
uniform float GrowthRate = 20;

void main()
{
    if (vLength > Time * GrowthRate) {
        discard;
        return;
    }
    FragColor = Color;
    gl_FragDepth = gl_FragCoord.z + DepthOffset;
}

-- Solid.VS

layout(location = 0) in vec4 Position;
layout(location = 1) in vec3 Normal;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat3 NormalMatrix;
uniform samplerBuffer CentroidTexture;
uniform float CullY;
uniform float Time;

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

uniform float Hue;
uniform vec3 Translate;
uniform vec3 Scale;
uniform float HueVariation = 0.05;
uniform float ExplosionStart = 3.0;

out vec3 vPosition;

void main()
{
    uint tetid = uint(gl_VertexID) / 12u;
    vec4 tetdata = texelFetch(CentroidTexture, int(tetid));
    vec3 tetcenter = tetdata.rgb;
    int neighbors = int(tetdata.a);
    if (tetcenter.y > CullY) {
        vColor = vec4(0);
    } else {
        float interiorHue = Hue;
        vFacetNormal = NormalMatrix * Normal;
        float hue   = (neighbors == 4) ? interiorHue : Hue;
        float sat   = (neighbors == 4) ? 1.0 : 0.3;
        float value = (neighbors == 4) ? 0.3 : 0.7;
        vec3 hsv = vec3(hue, sat, value);
        vColor =  vec4(HSVtoRGB(hsv), 1.0);
    }

    vec3 p = Position.xyz;
    vec3 scale = Scale;
    float implosion = 0;
    if (randhash(tetid, Time * 2) > 2.0) {
       implosion = 0.25;
    }

    float MaxHeight = 20.0;
    float BulgeDuration = 1.0;
    float Bulgeness = 0.05;
    if (Time > ExplosionStart - BulgeDuration) {
        float t = Time - (ExplosionStart - BulgeDuration);
        if (t > BulgeDuration) {
            t = BulgeDuration;
        }
        scale += (MaxHeight*0.5 - abs(tetcenter.y - MaxHeight*0.5)) * Bulgeness * sin(t * t / BulgeDuration);
    }

    if (Time > ExplosionStart) {
       float t = Time - ExplosionStart;

       float y = tetcenter.y / MaxHeight;
       float x = 1 + t * y * y;
       p = normalize(p) * length(p) * x;
       tetcenter = normalize(tetcenter) * length(tetcenter) * x;
       p.xz *= x;
       tetcenter.xz *= x;

       implosion += t * (1 + randhash(tetid, 1));
       implosion *= y;
       implosion = clamp(implosion, 0, 1);
    } 

    if (implosion >= 1) {
        gl_Position = vec4(0);
        return;
    }

    p = mix(p, tetcenter, implosion);

    vPosition = p * scale + Translate;
    gl_Position = Projection * Modelview * vec4(vPosition, 1);
}

-- Solid.FS

in vec3 vPosition;
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

    // Craptastic AO:
    float d = clamp((vPosition.y+5)/10, 0, 1);
    color *= d;

    FragColor = vec4(color, vColor.a);
}
