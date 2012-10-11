-- Facets.VS

layout(location = 0) in vec4 Position;

uniform mat4 Projection;
uniform mat4 Modelview;

uniform vec3 Translate = vec3(0);
uniform vec3 Scale = vec3(1);

out vec3 vPosition;

//out vec3 gPosition;
//out vec4 gColor;
//out vec3 gFacetNormal;

void main()
{
    //gColor = vec4(1,1,1,1);
    //gPosition = gFacetNormal =
    vPosition = Position.xyz * Scale + Translate;
    gl_Position = Projection * Modelview * vec4(vPosition, 1);
}

-- Facets.GS

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat3 NormalMatrix;
uniform mat4 Projection;
uniform mat4 Modelview;

in vec3 vPosition[3];

out vec4 gColor;
out vec3 gFacetNormal;
out float gAltitude;
out vec4 gePosition;
out float gIsRoof;
out vec2 gTexCoord;

uniform bool Smooth = false;

const vec3 ByteScale = 1.0 / vec3(255.0);
const float InverseMaxInt = 1.0 / 4294967295.0;
uniform int BuildingId;

float randhash(uint seed, float b)
{
    uint i=(seed^12345391u)*2654435769u;
    i^=(i<<6u)^(i>>26u);
    i*=2654435769u;
    i+=(i<<5u)^(i>>12u);
    return float(b * i) * InverseMaxInt;
}

void main()
{
    vec3 U = vPosition[2].xyz - vPosition[0].xyz;
    vec3 V = vPosition[1].xyz - vPosition[0].xyz;
    gFacetNormal = normalize(cross(U, V));
    gFacetNormal = NormalMatrix * gFacetNormal;

    //float p = vPosition[0].x + vPosition[0].y + vPosition[0].y;
    //float n = gFacetNormal.x + gFacetNormal.y + gFacetNormal.z;
    uint seed = uint(BuildingId);
    float sel = randhash(seed, 1.0);
    vec3 col = vec3(255.0) * sel;

    if (sel < 0.25) {
        col = vec3(243, 210, 129);
    } else if (sel < 0.5) {
        col = vec3(243, 206, 164);
    } else if (sel < 0.75) {
        col = vec3(161, 110, 45);
    } else {
        col = vec3(249, 219, 169);
    }

    gIsRoof = (gl_PrimitiveIDIn <= 1) ? 1 : 0;

    gColor = vec4(ByteScale * col, 1);

    bool odd = (gl_PrimitiveIDIn % 2) == 0;

    vec2 R = vec2(0,0);
    vec2 G = vec2(0,1);
    vec2 B = vec2(1,1);
    vec2 W = vec2(1,0);

    gTexCoord = odd ? R : B;
    gAltitude = vPosition[0].y;
    gePosition = Modelview * vec4(vPosition[0], 1);
    gl_Position = Projection * Modelview * vec4(vPosition[0], 1);
    EmitVertex();

    gTexCoord = odd ? G : W;
    gAltitude = vPosition[1].y;
    gePosition = Modelview * vec4(vPosition[1], 1);
    gl_Position = Projection * Modelview * vec4(vPosition[1], 1);
    EmitVertex();

    gTexCoord = odd ? B : R;
    gAltitude = vPosition[2].y;
    gePosition = Modelview * vec4(vPosition[2], 1);
    gl_Position = Projection * Modelview * vec4(vPosition[2], 1);
    EmitVertex();
    EndPrimitive();
}

-- Facets.FS

in vec4 gePosition;
in float gAltitude;
in vec4 gColor;
in vec3 gFacetNormal;
in vec2 gTexCoord;
in float gIsRoof;

out vec4 FragColor;
out vec3 Normal;
out vec4 Position;

uniform float DistanceScale; // 1 / (Far-Near)
uniform mat4 Modelview;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition = normalize(vec3(-1, 5, 1));
uniform vec3 AmbientMaterial = vec3(0.1, 0.1, 0.1);
uniform bool HasWindows = false;

void main()
{
    if (gColor.a == 0) {
        discard;
    }

    vec3 N = normalize(gFacetNormal);

    //vec3 L = NormalMatrix * normalize(LightPosition);
    vec3 L = normalize(LightPosition);

    float df = dot(-N, L);
    df = abs(df); // df = clamp(df, 0, 1);
    vec3 color = AmbientMaterial + df * gColor.rgb;

    // Craptastic AO:
    float d = clamp((gAltitude+5)/10, 0, 1);
    color *= d;

    if (HasWindows && gIsRoof == 0) {
        int h = int(gTexCoord.x * 9.0) % 2;
        int v = int(gTexCoord.y * 9.0) % 2;
        color *= 1.0 - (h*v);
    }

    FragColor = vec4(color, gColor.a);
    Normal = N;
    Position.xyz = gePosition.xyz;
    Position.w = length(gePosition) * DistanceScale;
}
