// FlashySlideshows background : an almost still gradient that drifts through a few
// hues over a couple of minutes. The quietest of the backgrounds , and the cheapest ,
// so it is also the safe choice on weak hardware.
// ShaderToy compatible , see shaders/background_aurora.frag for the conventions.

void mainImage(out vec4 fragColor,in vec2 fragCoord)
{
    vec2 uv = fragCoord / iResolution.xy;

    // one full trip around the hue circle every ~two minutes
    float t = iTime*0.05;

    vec3 top    = 0.5 + 0.5*cos(vec3(0.0,2.1,4.2) + t);
    vec3 bottom = 0.5 + 0.5*cos(vec3(0.0,2.1,4.2) + t + 1.1);

    top    = mix(vec3(0.02,0.02,0.05),top   ,0.30);
    bottom = mix(vec3(0.02,0.02,0.05),bottom,0.16);

    // a slow diagonal instead of a flat vertical , it reads less like a gradient tool
    float g = clamp(uv.y*0.85 + uv.x*0.15 + 0.05*sin(iTime*0.07),0.0,1.0);
    vec3 col = mix(bottom,top,g);

    // dither , 8 bit per channel gradients band badly on a large screen and this is
    // just below the noise floor of the display
    float dither = fract(sin(dot(fragCoord,vec2(12.9898,78.233)))*43758.5453);
    col += (dither-0.5)/255.0;

    vec2 v = uv-0.5;
    col *= 1.0 - 0.55*dot(v,v);

    fragColor = vec4(col,1.0);
}
