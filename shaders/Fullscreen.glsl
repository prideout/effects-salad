-- VS

void main()
{
    gl_Position = vec4(mix(vec2(-1.0), vec2(3.0), bvec2(gl_VertexID == 1, gl_VertexID == 2)), 0.0, 1.0);
}

-- FS

uniform bool ApplyVignette;
uniform bool ApplyScanline;
uniform bool ApplySolidColor;
uniform vec2 InverseViewport;
uniform vec4 SolidColor;
uniform sampler2D SourceImage;

out vec4 FragColor;
 
void main()
{
    vec2 tc = gl_FragCoord.xy * InverseViewport;

    vec4 c;
    if (not ApplySolidColor) {
        c = texture(tc, SourceImage);
    } else {
        c = SolidColor;
    }

    if (ApplyVignette) {
        float d = distance(tc, vec2(0.5));
        c.rgb *= 1 - 0.75 * d;
    }

    FragColor = c;
}
