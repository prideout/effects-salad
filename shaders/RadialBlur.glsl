-- VS

void main()
{
    gl_Position = vec4(mix(vec2(-1.0), vec2(3.0),
                           bvec2(gl_VertexID == 1, gl_VertexID == 2)), 0.0, 1.0);
}

-- FS

uniform vec2 InverseViewport;
uniform vec4 SolidColor;
uniform sampler2D SourceImage;
uniform sampler2D DepthImage;
out vec4 FragColor;
 
void main()
{
    vec2 tc = gl_FragCoord.xy * InverseViewport;

    vec4 c = textureLod(SourceImage, tc, 1.0);
    c += 2*textureLod(SourceImage, tc, 2.0);
    c += 3*textureLod(SourceImage, tc, 3.0);
    c += 4*textureLod(SourceImage, tc, 4.0);
    c.w *= 2;

    c.w *= length(c.xyz);

    FragColor = c;
}
