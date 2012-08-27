-- VS

void main()
{
    gl_Position = vec4(mix(vec2(-1.0), vec2(3.0), bvec2(gl_VertexID == 1, gl_VertexID == 2)), 0.0, 1.0);
}

-- FS

uniform vec4 Color;
out vec4 FragColor;
 
void main()
{
    FragColor = Color;
}
