-- Ground.FS
in vec3 vNormal;
in vec4 vPosition;
//in vec2 vUvCoord;
out vec4 FragColor;
uniform vec3 Eye;

// --------------------- ( NOISE LIB CODE ) --------------------------------
//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}
// --------------------- ( END NOISE LIB CODE ) --------------------------------

 // Ground.FS
uniform mat4 ViewMatrix;

void main()
{
#if 0
    vec3 light = normalize(vec3(1., 1., 1.) + Eye);
    //float s = vUvCoord.x + vUvCoord.y;
    //float r = min(1, max(10-sqrt(vPosition.x * vPosition.x + vPosition.z * vPosition.z), 0));
    //float r = clamp(1-.15*distance(eye, vPosition.xyz), 0., 1.);
    float r = clamp(.3+snoise(vPosition.xz/25.)+2*.5, 0., 1.);
    r = r*clamp(1-.045*distance(Eye.xyz, vPosition.xyz), 0., 1.);


    //FragColor = NdotHV * vec4(.7, .7, .2, 1.0);
    FragColor = r * vec4(.098, .063, .02, .2);
    //FragColor = r * vec4(.22, .15, .0, 1.0);
#endif

    //float s = vUvCoord.x + vUvCoord.y;
    float diffuseLight = .2;
    float ambientLight = .1;
    vec3 n = vNormal;

    vec3 l = (vec4(Eye, 1.0) - vPosition).xyz;
    //vec3 l = (vec4(-3, 2.0, 2, 1.0) - vPosition).xyz;
    float dist = length(l);
    float att = min(1.0, 1.0 / (dist*dist*.05));
    l = normalize(l);
    float d = max(0.0, dot(n, l));
    //d = 1.5;
    vec3 MaterialColor = vec3(.08, .60, .2);
    float occFactor = .5;
    FragColor = vec4(occFactor*(ambientLight*MaterialColor + att*d*diffuseLight*MaterialColor), 1.0);
}

-- Ground.VS
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
//layout(location = 2) in vec2 UvCoord;

out vec4 vPosition;
out vec3 vNormal;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
void main()
{
    vPosition = Position;
    gl_Position = Projection * Modelview * vPosition;
    vPosition = ModelMatrix * vPosition;
    vNormal = mat3(ModelMatrix) * normalize(Normal.xyz);
}

-- Stars.FS

in vec4 vPosition;
in float vBrightness;

out vec4 FragColor;

void main()
{
    //float s = vUvCoord.x + vUvCoord.y;
    FragColor = vBrightness * vec4(.5, .7, .9, 1.0);
}

-- Grass.FS

uniform vec3 Eye;

//in vec3 vNormal;
in vec4 vPosition;
in float vOcc;
//in vec2 vUvCoord;
out vec4 FragColor;

void main()
{
    //float s = vUvCoord.x + vUvCoord.y;
    float r = clamp(.2+1-.017*distance(Eye.xz, vPosition.xz), 0., 1.);

    float diffuseLight = .9;
    float ambientLight = .25;
    //vec3 n = vNormal;

    vec3 l = (vec4(Eye, 1.0) - vPosition).xyz;
    float dist = length(l);
    float att = min(1.0, 1.0 / (dist*dist*.01));
    l = normalize(l);
    float d = 1.0; //max(0.0, dot(n, l));
    vec3 MaterialColor = vec3(.05, .3, vOcc*vOcc*vOcc*.28);
    vec3 AmbMaterialColor = vec3(.05, .3, .3+vOcc*vOcc*vOcc*.48);
    FragColor = vec4(vOcc*(ambientLight*AmbMaterialColor + att*d*diffuseLight*MaterialColor), 1.0);

    //FragColor = r*MaterialColor;
}


-- Grass.VS
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
//layout(location = 2) in vec2 UvCoord;

out vec4 vPosition;
//out vec2 vUvCoord;
out vec3 vNormal;
out float vOcc;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform float Time;
uniform float WindAmount;

float snoise(vec2 v);

/*
uniform mat3 NormalMatrix;
*/
void main()
{
    /*
    vPosition = (Modelview * Position).xyz;
    gl_Position = Projection * Modelview * Position;
    vNormal = NormalMatrix * Normal;
    */
    vPosition = Position;
    vOcc = float(mod(gl_VertexID, 2));
    vPosition.xz += mod(gl_VertexID, 2) * vec2(0, WindAmount*snoise(vec2(Time+Position.x, Position.y) / 9.));
    //vPosition.xz += .4* mod(gl_VertexID, 2) * vec2(0, 5*snoise(vec2(gl_VertexID/1000.0,0.0)) + WindAmount);
    //vPosition.xz += .4* mod(gl_VertexID, 2) * vec2(cos(Time + gl_VertexID * .0000003), 0);
    gl_Position = Projection * Modelview * vPosition;
    vNormal = normalize(Normal.xyz);
}

// --------------------- ( NOISE LIB CODE ) --------------------------------
//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}
// --------------------- ( END NOISE LIB CODE ) --------------------------------

-- FallingLeafsTmp.FS

in vec4 vPosition;
in float vBrightness;

out vec4 FragColor;

uniform vec3 Eye;

void main()
{
    //float s = vUvCoord.x + vUvCoord.y;
    float dist = length(Eye - vPosition.xyz);
    FragColor = min(1.0, 1.0 / (dist*dist*.001)) * vec4(.8, .2, .2, 1.0);
}


-- FallingLeafsTmp.VS
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
//layout(location = 2) in vec2 UvCoord;

out vec4 vPosition;
out vec3 vNormal;
out float vBrightness;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

uniform float SizeMult;

void main()
{
    vPosition.xyz = Position.xyz;
    vPosition.w = 1.0;
    gl_Position = Projection * Modelview * vPosition;
    vPosition = ModelMatrix * vPosition;
    gl_PointSize = 2.5; ///SizeMult * Position.w;
    vBrightness = .1 + Position.w / 2.2;
    vNormal = mat3(ModelMatrix) * normalize(Normal.xyz);
}


-- Flies.FS

in vec4 vPosition;
in float vBrightness;

out vec4 FragColor;

uniform vec3 Eye;

void main()
{
    //float s = vUvCoord.x + vUvCoord.y;
    float dist = length(Eye - vPosition.xyz);
    FragColor = min(1.0, 1.0 / (dist*dist*.001)) * vec4(.1, .9, .2, 1.0);
}


-- Flies.VS
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
//layout(location = 2) in vec2 UvCoord;

out vec4 vPosition;
out vec3 vNormal;
out float vBrightness;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

uniform float SizeMult;

void main()
{
    vPosition.xyz = Position.xyz;
    vPosition.w = 1.0;
    gl_Position = Projection * Modelview * vPosition;
    vPosition = ModelMatrix * vPosition;
    gl_PointSize = SizeMult * Position.w;
    vBrightness = .1 + Position.w / 2.2;
    vNormal = mat3(ModelMatrix) * normalize(Normal.xyz);
}

-- Blossom.FS

in vec3 vNormal;
in vec4 vPosition;
in vec2 vUv;
in float vColorVar;

uniform vec3 Eye;

out vec4 FragColor;

vec3 blossom( float x, float y )
{
    float t = 0.0; //time;
    float bmps = 5.0;
    float a = atan(x,y);
    float r = sqrt(x*x+y*y);
    float s = 0.5 + 0.5*sin(bmps*a + t);
    float g = sin(1.57+bmps*a+t);
    float d = 0.7 + 0.3*sqrt(s) + 0.02*g*g;
    float h = r/d;
    float k = max(.5, h*3.0); //min(2.5, 1.0 / h);
    float f = 1.0-smoothstep( 0.98, 1.0, h );
    //k *= 1.0-0.5*(1.0-h)*smoothstep(0.95+0.05*h,1.0,sin(bmps*a+t));
    //return mix( vec3(.0), vec3(vColorVar+vColorVar*k,0.3*k,0.3*k), f );
    return mix( vec3(.0), vec3(0.4+0.4*k,0.3*k,0.3*k), f );
}


void main()
{
    vec3 MaterialColor = vec3(blossom(vUv.x, vUv.y)); //, vColorVar);
    if (MaterialColor.r == 0.0)
        discard;

    float vOcc = .6 + .4*vColorVar;
    float diffuseLight = 0.8;
    float ambientLight = 0.4;
    vec3 n = normalize(vNormal);
    vec3 l = Eye - vPosition.xyz;
    //vec3 l = vec3(-4, 5.0, 4) - vPosition.xyz;
    float dist = length(l);
    l = normalize(l);
    float d = max(0.0, dot(n, l));
    if (d == 0)
        d = max(0.0, dot(-n, l));
    float att = min(1.0, 1.0 / (dist*dist*dist*.003));
    FragColor = vec4(vOcc * (ambientLight*MaterialColor + att*d*diffuseLight*MaterialColor), 0.7);
    //FragColor.rgb = normalize(Eye);
    //FragColor.rgb = n;
    //FragColor.rgb = vec3(1 / (dist *dist*.5));
    //FragColor.rgb = vec3(d*att);
}



-- Blossom.VS
layout(location = 0) in vec3 Position;

out vec4 vPosition;
out vec3 vNormal;
out vec2 vUv;
out float vColorVar;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

uniform float Time;


uniform samplerBuffer LeafData;
uniform samplerBuffer LeafNormals;

void main()
{
    int id = int(gl_VertexID / 6);
    int dataId = id * 3;
    int uvId = int(mod(gl_VertexID, 6));

    // XXX: yuck, would be nice to have a single expression
    vUv.x = 2.0 * float(mod(uvId, 2)) - 1.0;
    if (uvId < 2 || uvId == 3)
        vUv.y = -1.0;
    else if (uvId > 3 || uvId == 2)
        vUv.y = 1.0;

    vNormal = texelFetch(LeafNormals, int(gl_VertexID / 12)).rgb;
    vPosition.xyz = Position.xyz;
    vPosition.w = 1.0;

    float startTime = texelFetch(LeafData, dataId).r;
    float growTime = texelFetch(LeafData, dataId+1).r;
    vColorVar = texelFetch(LeafData, dataId+2).r;

    if (Time < startTime) {
        gl_Position = vec4(0);
    } else {
        gl_Position = Projection * Modelview * vPosition;
    }

    //vNormal = mat3(Modelview) * vNormal;
    vPosition = ModelMatrix * vPosition;
}



-- Tree.FS

in vec3 vNormal;
in vec4 vPosition;
//in vec2 vUvCoord;

in float vOcc;

out vec4 FragColor;

uniform mat4 Projection;
uniform mat4 ViewMatrix;
uniform vec3 MaterialColor;

void main()
{
    //float s = vUvCoord.x + vUvCoord.y;
    float diffuseLight = 1;
    float ambientLight = .3;
    vec3 n = vNormal;
    vec3 l = normalize((vec4(4, 2.0, -4, 1.0)).xyz); // - vPosition).xyz);
    float d = max(0.0, dot(n, l));
    //d = 1.5;
    FragColor = vec4(vOcc * (ambientLight*MaterialColor + d*diffuseLight*MaterialColor), 1.0);
    //FragColor = vec4(d,d,d, 1.0);
    //FragColor = vec4(n, 1.0);
}

-- Tube.VS
// --------------------- ( NOISE LIB CODE ) --------------------------------
//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}
// --------------------- ( END NOISE LIB CODE ) --------------------------------

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

out vec4 vPosition;
out vec3 vNormal;

out float vOcc;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

uniform int Slices;
uniform int VertsPerSlice;
uniform float Time;
uniform float TimeToGrow;

uniform float MinOcc;

uniform samplerBuffer Centerline;
uniform samplerBuffer Frames;
uniform samplerBuffer Scales;

void main()
{
    int id = int(gl_VertexID / VertsPerSlice);

    vOcc = clamp(float(id*5) / Slices, MinOcc, 1.0);

    // the time when this slices starts
    float sliceTime = (TimeToGrow / Slices);
    float sliceStartTime = id * sliceTime;

    // The percent complete for the current slice given the current time
    // Segments at the end grow faster so they can reach their max size by the end of the grow time
    // if there is a tapering to the scales, this effect shouldn't be noticable
    // I'm not sure if I like this, because the trunk grows too slowly
    float s = mix(0.0, 1.0, clamp((Time - sliceStartTime) / (TimeToGrow - sliceStartTime), 0., 1.));
    float s0 = mix(0.0, 1.0, clamp((Time - (id-1)*sliceTime) / (TimeToGrow - (id-1)*sliceTime), 0., 1.));

    mat3 basis = mat3(texelFetch(Frames, id*3+0).rgb,
                      texelFetch(Frames, id*3+1).rgb,
                      texelFetch(Frames, id*3+2).rgb);

    float scale = s*texelFetch(Scales, id).r;
    vPosition.w = 1.0;

    vPosition.xyz = Position.xyz * scale; // * (5 + sin(id/2));

    // add noise to the current ring
    //vPosition.xyz *=  1 + .2*snoise(vec2(id/4.,0)) + .2 * snoise(vec2(mod(id, Slices) / 4.0));

    float oldScale = texelFetch(Scales, id-1).r;
    if ((s == 0 && s0 > 0)) { //id > 0 && scale == 0 && oldScale > 0) {
        float pct = clamp((Time-(id-1)*sliceTime) / sliceTime, 0., 1.);
        mat3 basis2 = mat3(texelFetch(Frames, (id-1)*3+0).rgb,
                           texelFetch(Frames, (id-1)*3+1).rgb,
                           texelFetch(Frames, (id-1)*3+2).rgb);
        vPosition.xyz = mix(basis2*vPosition.xyz, basis*vPosition.xyz, pct);
        vPosition.xyz += mix(texelFetch(Centerline, id-1).rgb, texelFetch(Centerline, id).rgb, pct);
        vNormal = mat3(ModelMatrix) * mix(basis2*Normal, basis*Normal, pct);
    } else {
        vPosition.xyz = basis*vPosition.xyz;
        vPosition.xyz += texelFetch(Centerline, id).rgb;
        vNormal = mat3(ModelMatrix) * basis*Normal;
    }

    gl_Position = Projection * Modelview * vPosition;
    //vNormal = mat3(Modelview) * vNormal;
    vPosition = ModelMatrix * vPosition;
    vNormal = normalize(vNormal);
}


-- Blur.FS

//in vec3 vNormal;
in vec4 vPosition;
in vec2 vUvCoord;
out vec4 FragColor;

uniform sampler2D Tex;

void main()
{
    float s = vUvCoord.x + vUvCoord.y;
    vec4 tval = texture(Tex, vUvCoord);
    //FragColor = vec4(1., 1., 1., 1.0); //tval;
    FragColor = tval;
}


-- Blur.VS
layout(location = 0) in vec2 Position;
layout(location = 2) in vec2 UvCoord;

out vec4 vPosition;
out vec2 vUvCoord;
//out vec3 vNormal;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
/*
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
    //vPosition.z = -2;
    vPosition.w = 1.0;
    vUvCoord = UvCoord;
    gl_Position = Projection * Modelview * vPosition;
}
