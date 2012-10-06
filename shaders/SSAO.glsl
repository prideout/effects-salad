-- VS

void main()
{
    gl_Position = vec4(mix(vec2(-1.0), vec2(3.0), bvec2(gl_VertexID == 1, gl_VertexID == 2)), 0.0, 1.0);
}

-- FS

uniform vec2 InverseViewport;
uniform vec4 SolidColor;

uniform sampler2D SourceImage;
uniform sampler2D DepthImage;
uniform sampler2D NormalsImage;
uniform sampler2D PositionsImage;

uniform float Time = 0;

out vec4 FragColor;
 
void main()
{
    vec2 tc = gl_FragCoord.xy * InverseViewport;

    vec4 c = texture(SourceImage, tc);

    //float z = (texture(DepthImage, tc).r - 0.99) * 100.0;
    float z = texture(PositionsImage, tc).w;

    vec3 n = texture(NormalsImage, tc).rgb;
    vec3 p = texture(PositionsImage, tc).rgb;

    //c = vec4(z,z,z,1);
    c = vec4(n,1);
    //c = vec4(p,1);  

    FragColor = c;
}
