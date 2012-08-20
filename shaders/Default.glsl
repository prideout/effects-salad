-- Simple.FS

//in vec3 vNormal;
in vec3 vPosition;
out vec4 FragColor;

void main()
{
    FragColor = vec4(.1, .4, .9, 1.0);
}


-- Simple.VS
layout(location = 0) in vec2 Position;
//layout(location = 1) in vec3 Normal;

out vec4 vPosition;
//out vec3 vNormal;

/*
uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
*/
void main()
{
    /*
    vPosition = (Modelview * Position).xyz;
    gl_Position = Projection * Modelview * Position;
    vNormal = NormalMatrix * Normal;
    */
    vPosition.xy = Position.xy;
    vPosition.z = 0.1;
    vPosition.w = 1;
    gl_Position.xy = Position.xy;
    gl_Position.z = 0.1;
    gl_Position.w = 1;
    //vNormal = Normal;
}



-- Lit.VS

layout(location = 0) in vec4 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TubeCoord;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTubeCoord;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

void main()
{
    vTubeCoord = TubeCoord;
    vPosition = (Modelview * Position).xyz;
    gl_Position = Projection * Modelview * Position;
    vNormal = NormalMatrix * Normal;
}

-- Lit.EnvMap.VS

layout(location = 0) in vec4 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TubeCoord;

out vec4 evPosition;
out vec3 evNormal;
out vec2 evTubeCoord;

void main()
{
    evTubeCoord = TubeCoord;
    evPosition = Position;
    evNormal = Normal;
    gl_Position = vec4(0, 0, 0, 1); // TODO Is this really required?
}

-- Plasma.EnvMap.VS

layout(location = 0) in vec4 Position;
layout(location = 2) in vec2 TubeCoord;

uniform mat4 Projection;
uniform mat4 Modelview;

out vec2 evTexCoord;
out vec4 evPosition;

void main()
{
    evTexCoord = vec2( 20.0, 1.0 ) * TubeCoord;
    evPosition = Position;
    gl_Position = vec4(0, 0, 0, 1); // TODO Is this really required?
}

-- Plasma.EnvMap.GS

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 Projection;
uniform mat4 Modelviews[6];

in vec4 evPosition[3];
in vec2 evTexCoord[3];

out vec2 vTexCoord;
out vec3 vPosition;

void main()
{
    // 6 faces of the cubemap:
    for (int face = 0; face < 6; face++) {
        gl_Layer = face;

        mat4 MV = Modelviews[face];

        // 3 corners of the triangle:
        for (int i = 0; i < 3; i++) {
            vec4 P = MV * evPosition[i];
            vPosition   = P.xyz / P.w;
            vTexCoord  = evTexCoord[i];
            gl_Position = Projection * P;
            EmitVertex();
        }

        EndPrimitive();
    }
}

-- Lit.EnvMap.GS

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 Projection;
uniform mat4 Modelviews[6];
uniform mat3 NormalMats[6];

in vec4 evPosition[3];
in vec3 evNormal[3];
in vec2 evTubeCoord[3];

out vec2 vTubeCoord;
out vec3 vNormal;
out vec3 vPosition;

void main()
{
    // 6 faces of the cubemap:
    for (int face = 0; face < 6; face++) {
        gl_Layer = face;

        mat4 MV = Modelviews[face];
        mat3 N  = NormalMats[face];

        // 3 corners of the triangle:
        for (int i = 0; i < 3; i++) {
            vec4 P = MV * evPosition[i];
            vPosition   = P.xyz / P.w;
            vNormal     = N * evNormal[i];
            vTubeCoord  = evTubeCoord[i];
            gl_Position = Projection * P;
            EmitVertex();
        }

        EndPrimitive();
    }
}

-- Lit.FS

in vec2 vTubeCoord;
in vec3 vNormal;
in vec3 vPosition;
out vec4 FragColor;

uniform mat4 ViewMatrix;
uniform vec3 AmbientMaterial = vec3(0.1, 0.1, 0.12);
uniform vec3 SpecularMaterial = vec3(1.0, 1.0, 1.0);
uniform vec3 FrontMaterial = vec3(0.5, 0.5, 0.5); // Inside
uniform vec3 BackMaterial = vec3(0.9, 0.9, 0.1); // Outside
uniform float Shininess = 50;
uniform float Time;
uniform float DollyPosition;
uniform float Alpha;
uniform float RingLightPositions[MaxRingLightCount];
uniform vec3  RingLightColor;
uniform float RingLightThickness;

uniform vec3 LightPositions[MaxLightCount]; // These are in eye space
uniform float LightAttenuation[MaxLightCount];

uniform int  LightCount;

void main()
{
    vec3 N = normalize(vNormal);
    if (!gl_FrontFacing) {
       N = -N;
    }

    vec3 V = vPosition; // Vertex position in eye space
    vec3 Eye = vec3(0, 0, 1);
    vec3 color = gl_FrontFacing ? FrontMaterial : BackMaterial;
    vec3 lighting = AmbientMaterial;

    for (int i = 0; i < LightCount; i++) {
        vec3 Pli = LightPositions[i];
        vec3 Vp = normalize(Pli - V);
        vec3 H = normalize(Vp + Eye);
        float df = max(0.0, dot(N, Vp));
        float sf = max(0.0, dot(N, H));
        sf = pow(sf, Shininess);
        vec3 a = Pli - V;
        float att = 1.0 / (1.0 + LightAttenuation[i] * dot(a,a));
        sf *= att;
        df *= att;
        lighting += df * color;
        if (gl_FrontFacing) {
            lighting += sf * SpecularMaterial;
        }
    }

    float band = mod(vTubeCoord.s, 0.005);
    if (band < 0.005) {
        band /= 0.005;
        float df = 2.0 * abs(band - 0.5);

        const float A = 0.1; // thickness

        float E = fwidth(df);
        if (df > A - E && df < A + E)
            df = mix(A, 1.0, smoothstep(A - E, A + E, df));
        else if (df < A) df = 0.0;
        else df = 1.0;

        lighting *= df;

    }
    band = mod(vTubeCoord.t+sin(Time*.05) + sin(.5*Time*vTubeCoord.s), 0.1);
    if (band < 0.1) {
        band /= 0.1;
        band = abs(band - 0.5);
        float sf = band;
        float E = fwidth(sf);
        float F = 0.45;
        if (sf > F - E && sf < F + E)
            sf = clamp(F * (sf - F + E) / E, 0.0, 1.0);
        else
            sf = step(F, sf);
        lighting *= 1-sf;
    }

    FragColor = vec4(lighting, Alpha);
}


-- Constant.VS

layout(location = 0) in vec4 Position;

uniform mat4 Projection;
uniform mat4 Modelview;

void main()
{
    gl_Position = Projection * Modelview * Position;
}

-- Constant.FS

out vec4 FragColor;
uniform vec4 Color;

void main()
{
    FragColor = vec4(Color);
}


-- Plasma.VS

layout(location = 0) in vec4 Position;
layout(location = 2) in vec2 TubeCoord;

uniform mat4 Projection;
uniform mat4 Modelview;

out vec2 vTexCoord;

void main()
{
    vTexCoord = vec2( 20.0, 1.0 ) * TubeCoord;
    gl_Position = Projection * Modelview * Position;

}

-- Plasma.FS

uniform float Time;
			
uniform float fogDensity = 0.45;
uniform vec3 fogColor = vec3(0, 0, 0);

uniform sampler2D cloud;
uniform sampler2D plasma;

in vec2 vTexCoord;
out vec4 FragColor;

void main( void ) {

    vec2 position = -1.0 + 2.0 * vTexCoord;

    vec4 noise = texture( cloud, vTexCoord );
    vec2 T1 = vTexCoord + vec2( 1.5, -1.5 ) * Time * 0.02;
    vec2 T2 = vTexCoord + vec2( -0.5, 2.0 ) * Time * 0.01;
				
    T1.x += noise.x * 2.0;
    T1.y += noise.y * 2.0;
    T2.x -= noise.y * 0.2;
    T2.y += noise.z * 0.2;
				
    float p = texture( cloud, T1 * 2.0 ).a;
				
    vec4 color = texture( plasma, T2 * 2.0 ).bgra;
    vec4 temp = color * ( vec4( p, p, p, p ) * 2.0 ) + ( color * color - 0.1 );
				
    if( temp.r > 1.0 ){ temp.bg += clamp( temp.r - 2.0, 0.0, 100.0 ); }
    if( temp.g > 1.0 ){ temp.rb += temp.g - 1.0; }
    if( temp.b > 1.0 ){ temp.rg += temp.b - 1.0; }
				
    FragColor = temp;

    float depth = gl_FragCoord.z * 1; // TODO some sort of auto-normalization?
    const float LOG2 = 1.442695;
    float fogFactor = exp2( - fogDensity * fogDensity * depth * depth * LOG2 );
    fogFactor = 1.0 - clamp( fogFactor, 0.0, 1.0 );
				
    FragColor = mix( FragColor, vec4( fogColor, temp.a ), fogFactor );
}

-- Quad.VS

layout(location = 0) in vec2 Position;
layout(location = 3) in vec2 TexCoord;
out vec2 vTexCoord;

void main()
{
    vTexCoord = TexCoord;
    gl_Position = vec4(Position, 0, 1);
}

-- Blur.FS

in vec2 vTexCoord;
layout(location = 0) out vec4 FragColor;
uniform sampler2D Sampler;
uniform vec2 Offsets[3];
uniform float Weights[3];
uniform int Uber = 0;

void main()
{
    FragColor = vec4(0);
    for (int i = 0; i < 3; i++) {
        float w = Weights[i];
        vec2 o = Offsets[i];
        FragColor += w * texture(Sampler, vTexCoord + o);
    }

    if (Uber > 0) {
        float coef = Weights[0] * .2;
        vec2 offset = Offsets[0];
        for (float i = 2.; i < 10.; i+=3.) {
            FragColor += coef * texture(Sampler, vTexCoord + i*offset);
            FragColor += coef * texture(Sampler, vTexCoord - i*offset);
            coef *= .99;
        }
        /* old stage-2 uber bloom
        */
        for (float i = 10.; i < 60.; i+=2.) {
            FragColor += vec4(.4,.4,1.,1.)*coef * texture(Sampler, fract(vTexCoord + i*offset));
            FragColor += vec4(.4,.4,1.,1.)*coef * texture(Sampler, fract(vTexCoord - i*offset));
            coef *= .95;
        }
    }
}

-- Quad.FS

in vec2 vTexCoord;
layout(location = 0) out vec4 FragColor;
uniform sampler2D Sampler;
uniform float Alpha = 1.0;
uniform int DoVignette = 0;
uniform int DoGrain = 0;
uniform float GrainOffset = 0.0;
uniform float GrainIntensity = 0.1;

vec3 v;
float rand(vec2 n)
{
  return 0.5 + 0.5 *
     fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453 * GrainOffset);
}

vec4 grain(vec4 color)
{
    if (DoGrain != 0) {
        color = color - GrainIntensity * (.5 - rand(vTexCoord));
    }

    return color;
}

vec4 vignette(vec4 color)
{
    if (DoVignette != 0) {
        float d = abs(length(vTexCoord - .5));
        if (d > 0.0)
            color = vec4(vec3(clamp(1.-(.2*d),0.,1.)*color), 1.0);
    }
    
    return color;
}

void main()
{
    FragColor = texture(Sampler, vTexCoord);
    FragColor.a *= Alpha;
    FragColor = grain(vignette(FragColor)); 
}

-- Hipass.FS

in vec2 vTexCoord;
layout(location = 0) out vec4 FragColor;
uniform sampler2D Sampler;
uniform float Threshold = 1.0;
const vec3 Black = vec3(0, 0, 0);

void main()
{
    vec3 c = texture(Sampler, vTexCoord).rgb;
    float gray = dot(c,c);
    FragColor = vec4(gray > Threshold ? c : Black, 1);
}

-- Shiny.VS

layout(location = 0) in vec4 Position;
layout(location = 1) in vec3 Normal;
layout(location = 3) in vec2 TexCoord;

out vec3 vPosition;
out vec3 vNormal;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

void main()
{
    vPosition = (Modelview * Position).xyz;
    gl_Position = Projection * Modelview * Position;
    vNormal = NormalMatrix * Normal;
}

-- Shiny.FS

in vec3 vNormal;
in vec3 vPosition;
out vec4 FragColor;

uniform vec3 AmbientMaterial = vec3(0.1, 0.1, 0.12);
uniform vec3 SpecularMaterial = vec3(1.0, 1.0, 1.0);
uniform vec3 DiffuseMaterial = vec3(1.0, 0.0, 0.75);
uniform float Shininess = 50;

uniform samplerCube Env;

uniform vec3 LightPositions[MaxLightCount]; // These are in eye space
uniform float LightAttenuation[MaxLightCount];

uniform int LightCount;

const float Eta = 0.66; // Ratio of indices of refreaction
const float FresnelPower = 4.0;
const float F = ((1.0-Eta) * (1.0-Eta)) / ((1.0+Eta)*(1.0+Eta));

void main()
{
    vec3 N = normalize(vNormal);
    if (!gl_FrontFacing) {
       N = -N;
    }

    vec3 V = vPosition; // Vertex position in eye space
    vec3 Eye = vec3(0, 0, 1);
    vec3 color = DiffuseMaterial;
    vec3 lighting = AmbientMaterial;

    vec3 i = normalize(V);
    float ratio = F+(1-F)*pow((1-dot(-i,N)),FresnelPower);

    for (int i = 0; i < LightCount; i++) {
        vec3 Pli = LightPositions[i];
        vec3 Vp = normalize(Pli - V);
        vec3 H = normalize(Vp + Eye);
        float df = max(0.0, dot(N, Vp));
        float sf = max(0.0, dot(N, H));
        sf = pow(sf, Shininess);
        vec3 a = Pli - V;
        float att = 1.0 / (1.0 + LightAttenuation[i] * dot(a,a));
        sf *= att;
        df *= att;
        lighting += df * color;
        lighting += sf * SpecularMaterial;
    }
    FragColor = vec4(lighting, 1);

    vec3 texcolor = 5*texture(Env, N).rgb;
    FragColor.rgb = mix(lighting, texcolor, ratio);
}


-- Adder.FS

in vec2 vTexCoord;
layout(location = 0) out vec4 FragColor;
uniform sampler2D Sampler0;
uniform sampler2D Sampler1;
uniform sampler2D Sampler2;
uniform sampler2D Sampler3;
uniform sampler2D Sampler4;
uniform float Alpha0 = 1.0;
uniform float Alpha1 = 1.0;
uniform float Alpha2 = 1.0;
uniform float Alpha3 = 1.0;
uniform float Alpha4 = 1.0;

void main()
{
    FragColor = Alpha0*texture(Sampler0, vTexCoord) +
                Alpha1*texture(Sampler1, vTexCoord) +
                Alpha2*texture(Sampler2, vTexCoord) +
                Alpha3*texture(Sampler3, vTexCoord) + 
                Alpha4*texture(Sampler4, vTexCoord);
}

 
