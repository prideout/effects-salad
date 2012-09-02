-- VS

void main()
{
    gl_Position = vec4(mix(vec2(-1.0), vec2(3.0), bvec2(gl_VertexID == 1, gl_VertexID == 2)), 0.0, 1.0);
}

-- FS

uniform bool ApplyVignette;
uniform bool ApplyScanLines;
uniform bool ApplyTeleLines;
uniform bool ApplySolidColor;
uniform bool ApplyCopyDepth;
uniform vec2 InverseViewport;
uniform vec4 SolidColor;
uniform sampler2D SourceImage;
uniform sampler2D DepthImage;

uniform vec4 ScanLineColor = vec4(0.2);
uniform float TeleLineFreq = 10;

out vec4 FragColor;
 
void main()
{
    if (ApplyScanLines) {
        if (mod(gl_FragCoord.y, 2) < 1) {
           FragColor = ScanLineColor;
           return;
        } 
    }

    vec2 tc = gl_FragCoord.xy * InverseViewport;

    vec4 c;
    if (!ApplySolidColor) {
        c = texture(SourceImage, tc);
    } else {
        c = SolidColor;
    }

    if (ApplyTeleLines) {
        float a = mod(gl_FragCoord.y, TeleLineFreq) / TeleLineFreq;
        c.rgb *= 0.5 + 0.25 * (sin(a * 6.28) + 1.0);
    }

    if (ApplyVignette) {
        float d = distance(tc, vec2(0.5));
        c.rgb *= 1 - 0.75 * d;
    }
    
    if (ApplyCopyDepth) {
       gl_FragDepth = texture(DepthImage, tc).x;
    }

    FragColor = c;
}
