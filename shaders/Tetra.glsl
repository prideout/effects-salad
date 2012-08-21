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
    FragColor = vec4(0.8, 0.9, 1.0, 0.75);
}
