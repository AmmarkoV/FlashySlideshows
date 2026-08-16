// FlashySlideshows background : the classic sine plasma , toned down to something
// that can sit behind a photograph for an hour without becoming tiring.
// ShaderToy compatible , see shaders/background_aurora.frag for the conventions.
//
// iMouse is wired up , so moving the mouse pulls the swirl around with it.

void mainImage(out vec4 fragColor,in vec2 fragCoord)
{
    vec2 p = (fragCoord - 0.5*iResolution.xy)/iResolution.y;
    vec2 m = (iMouse.xy  - 0.5*iResolution.xy)/iResolution.y;

    float t = iTime*0.15;
    float d = length(p-m);
    float swirl = 0.5*sin(3.0*d - iTime*0.5);

    float v = 0.0;
    v += sin(p.x*3.0 + t + swirl);
    v += sin(p.y*2.4 - t*1.3);
    v += sin((p.x+p.y)*2.2 + t*0.7);
    v += sin(length(p)*5.0 - t*1.6);

    vec3 col = 0.5 + 0.5*cos(vec3(0.0,2.1,4.2) + v*1.2 + iTime*0.08);

    // pull the whole thing down towards a dark blue so the photos stay the brightest
    // thing on the screen
    col = mix(vec3(0.02,0.03,0.07),col,0.28);

    vec2 uv = fragCoord/iResolution.xy;
    vec2 vg = uv-0.5;
    col *= 1.0 - 0.7*dot(vg,vg);

    fragColor = vec4(col,1.0);
}
