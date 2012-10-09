-- Sky.FS

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


//in vec3 vNormal;
in vec4 vPosition;
in vec2 vUvCoord;
out vec4 FragColor;
uniform float Azimuth;
uniform float Altitude;

void main()
{
    // compute the azimuth and altitude (thank you, undergrad astronomy :)
    
    // TODO: Azimuth should be spherically warped; maybe a plane deformation
    // Reflect noise across the sky, given that Azimuth in [-1,1] and uv.u in [-1,1]
    float u = Azimuth + .4*vUvCoord.x;
    if (u < 0) {
        u = -Azimuth - .4*vUvCoord.x;
    } 
    if (u > 1) {
        u = 2 - u;
    }
    // adjust the noise scale
    u *= 1;

    FragColor = vec4(.0, .0, .2*snoise(vec2(u, vUvCoord.y)), 1.0);
    //FragColor = vec4(.0, .0, .2*snoise(vec2(u, Altitude+vUvCoord.y)), 1.0);
}

-- Sky.GS

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 evPosition[3];
in vec2 evUvCoord[3];

out vec2 vUvCoord;
out vec4 vPosition;

void main()
{
    // 3 corners of the triangle:
    for (int i = 0; i < 3; i++) {
        vUvCoord = evUvCoord[i];
        vPosition = evPosition[i];
        gl_Position = evPosition[i];
        EmitVertex();
    }

    EndPrimitive();
}

-- Sky.VS
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 UvCoord;

out vec4 evPosition;
out vec2 evUvCoord;
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
    evPosition = Position;
    evPosition.z = .999990;
    evPosition.w = 1.0;
    evUvCoord = UvCoord;
    //gl_Position = evPosition;
    //gl_Position = Projection * Modelview * evPosition;
    gl_Position = evPosition;
    //vNormal = normalize(Normal.xyz);
}


