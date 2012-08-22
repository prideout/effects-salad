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
    FragColor = vec4(1, 0, 0, 0.25);
}

-- Solid.VS

layout(location = 0) in vec4 Position;

out vec4 vPosition;

void main()
{
    vPosition = Position;
}

-- Solid.GS

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 Projection;
uniform mat4 Modelview;

in vec4 vPosition[3];

void main()
{
    gl_Position = Projection * Modelview * vPosition[0];
    EmitVertex();

    gl_Position = Projection * Modelview * vPosition[1];
    EmitVertex();

    gl_Position = Projection * Modelview * vPosition[2];
    EmitVertex();
    EndPrimitive();
}


-- Solid.FS

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.8, 0.9, 1.0, 1.0);
}
