-- XZPlane.VS

uniform float Y = 0;
uniform float Scale = 40;

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
