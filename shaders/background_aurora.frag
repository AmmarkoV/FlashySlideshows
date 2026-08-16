// FlashySlideshows background : slow northern-lights curtains.
// ShaderToy compatible , the file is pasted behind the preamble in
// src/visuals/shadertoy.cpp which declares iResolution / iTime / iMouse / iChannel0..3
//
// Backgrounds sit behind the photos , so they stay dark and slow on purpose , a
// bright or fast background pulls the eye away from the picture it is framing.

float hash(vec2 p)
{
    return fract(sin(dot(p,vec2(127.1,311.7)))*43758.5453123);
}

float noise(vec2 p)
{
    vec2 i = floor(p);
    vec2 f = fract(p);
    f = f*f*(3.0-2.0*f);
    return mix(mix(hash(i+vec2(0.0,0.0)),hash(i+vec2(1.0,0.0)),f.x),
               mix(hash(i+vec2(0.0,1.0)),hash(i+vec2(1.0,1.0)),f.x),f.y);
}

float fbm(vec2 p)
{
    float v = 0.0 , a = 0.5;
    int i;
    for (i=0; i<5; i++)
    {
        v += a*noise(p);
        p *= 2.02;
        a *= 0.5;
    }
    return v;
}

void mainImage(out vec4 fragColor,in vec2 fragCoord)
{
    vec2 uv = fragCoord / iResolution.xy;
    float t = iTime * 0.06;

    // deep night sky , a touch lighter towards the bottom
    vec3 col = mix(vec3(0.01,0.02,0.05),vec3(0.03,0.05,0.11),uv.y);

    // three curtains at different depths , each one a vertical band of fbm that
    // wanders sideways and fades out towards the top of the screen
    int i;
    for (i=0; i<3; i++)
    {
        float fi    = float(i);
        float depth = 0.6 + fi*0.35;
        float x     = uv.x*1.6 + sin(t*1.7 + fi*2.1)*0.35 - fi*0.4;

        float band  = fbm(vec2(x*2.5,uv.y*1.2 - t*(1.0+fi*0.4)));
        float sheet = smoothstep(0.45,0.85,band);

        // fragCoord has y going up , so uv.y==1 is the top of the screen : the
        // curtains hang from up there and dissolve on their way down
        sheet *= smoothstep(0.0,0.6,uv.y);
        sheet *= 0.75/depth;

        vec3 tint = mix(vec3(0.10,0.85,0.45),vec3(0.25,0.35,0.95),fi*0.5);
        col += sheet*tint;
    }

    // a very sparse dusting of stars , only where the curtains are not
    float star = step(0.9985,hash(floor(fragCoord)));
    col += star*vec3(0.7,0.75,0.9)*(0.5+0.5*sin(iTime*2.0+hash(floor(fragCoord))*30.0));

    // vignette , keeps the corners out of the way of the photos
    vec2 v = uv-0.5;
    col *= 1.0 - 0.7*dot(v,v);

    fragColor = vec4(col,1.0);
}
