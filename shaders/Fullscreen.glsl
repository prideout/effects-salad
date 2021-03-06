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
uniform bool ApplyBrightness;
uniform vec2 InverseViewport;
uniform vec4 SolidColor;
uniform sampler2D SourceImage;
uniform sampler2D DepthImage;
uniform vec4 ScanLineColor = vec4(0.);
uniform float TeleLineFreq = 10;
uniform float Time = 0;
uniform float Brightness = 1.0;

out vec4 FragColor;
 
void main()
{
    vec2 tc = gl_FragCoord.xy * InverseViewport;

    vec4 c;
    if (!ApplySolidColor) {
        c = texture(SourceImage, tc);
    } else {
        c = SolidColor;
    }

    if (ApplyTeleLines) {
        float a = mod(gl_FragCoord.y, TeleLineFreq) / TeleLineFreq;
        a -= 5*Time;
        c.rgb *= 0.75 + 0.125 * (sin(a * 6.28) + 1.0);
    }

    if (ApplyVignette) {
        float d = distance(tc, vec2(0.5));
        c.rgb *= 1 - 0.75 * d;
    }
    
    if (ApplyCopyDepth) {
       gl_FragDepth = texture(DepthImage, tc).x;
    }

    if (ApplyScanLines) {
        if (mod(gl_FragCoord.y, 2) < 1) {
           c = mix(c, ScanLineColor, .5);
        } 
    }

    if (ApplyBrightness) {
        c = mix(SolidColor, c, Brightness);
    }

    FragColor = c;
}
