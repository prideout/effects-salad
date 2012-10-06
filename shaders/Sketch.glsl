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
out vec3 gPosition;
out vec4 gePosition;

uniform bool Smooth = false;

void main()
{
    vec3 A = vPosition[2].xyz - vPosition[0].xyz;
    vec3 B = vPosition[1].xyz - vPosition[0].xyz;
    gFacetNormal = normalize(cross(A, B));
    bool horiz = abs(gFacetNormal.y) > 0.01;
    bool smoothie = Smooth && !horiz;
    gFacetNormal = NormalMatrix * gFacetNormal;

    gColor = vec4(1, 1, 0.9, 1);
    if (horiz) {
      //gColor = vec4(1, 0.4, 0.4, 1);
    }   

    if (smoothie) gFacetNormal = NormalMatrix * normalize(vec3(vPosition[0].x, 0, vPosition[0].z));
    gPosition = vPosition[0];
    gePosition = Modelview * vec4(vPosition[0], 1);
    gl_Position = Projection * Modelview * vec4(vPosition[0], 1);
    EmitVertex();

    if (smoothie) gFacetNormal = NormalMatrix * normalize(vec3(vPosition[1].x, 0, vPosition[1].z));
    gPosition = vPosition[1];
    gePosition = Modelview * vec4(vPosition[1], 1);
    gl_Position = Projection * Modelview * vec4(vPosition[1], 1);
    EmitVertex();

    if (smoothie) gFacetNormal = NormalMatrix * normalize(vec3(vPosition[2].x, 0, vPosition[2].z));
    gPosition = vPosition[2];
    gePosition = Modelview * vec4(vPosition[2], 1);
    gl_Position = Projection * Modelview * vec4(vPosition[2], 1);
    EmitVertex();
    EndPrimitive();
}

-- Facets.FS

in vec4 gePosition;
in vec3 gPosition;
in vec4 gColor;
in vec3 gFacetNormal;

out vec4 FragColor;
out vec3 Normal;
out vec4 Position;

uniform float DistanceScale; // 1 / (Far-Near)
uniform mat4 Modelview;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition = normalize(vec3(0, 1, 1));
uniform vec3 AmbientMaterial = vec3(0.1, 0.1, 0.1);

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
    float d = clamp((gPosition.y+5)/10, 0, 1);
    color *= d;

    FragColor = vec4(color, gColor.a);
    Normal = N;
    Position.xyz = gePosition.xyz;
    Position.w = length(gePosition) * DistanceScale;
}
