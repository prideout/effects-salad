-- Flies.FS

//in vec3 vNormal;
in vec4 vPosition;
//in vec2 vUvCoord;
out vec4 FragColor;

void main()
{
    //float s = vUvCoord.x + vUvCoord.y;
    FragColor = vec4(.1, .9, .2, 1.0);
}


-- Flies.VS
layout(location = 0) in vec4 Position;
//layout(location = 1) in vec3 Normal;
//layout(location = 2) in vec2 UvCoord;

out vec4 vPosition;
//out vec2 vUvCoord;
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
    vPosition = Position;
    gl_Position = Projection * Modelview * vPosition;
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
