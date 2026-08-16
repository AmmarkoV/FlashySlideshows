// FlashySlideshows background : out of focus lights drifting upwards , the sort of
// thing a photograph gets when the lens is wide open and the subject is not.
// ShaderToy compatible , see shaders/background_aurora.frag for the conventions.

float hash(float n) { return fract(sin(n)*43758.5453123); }

void mainImage(out vec4 fragColor,in vec2 fragCoord)
{
    // work in an aspect corrected space so the circles are round on any window
    vec2 uv = (fragCoord - 0.5*iResolution.xy) / iResolution.y;
    float t = iTime*0.05;

    // dark blue-grey backdrop with a soft diagonal falloff
    vec3 col = mix(vec3(0.02,0.03,0.06),vec3(0.07,0.05,0.10),clamp(uv.y+0.5,0.0,1.0));

    int i;
    for (i=0; i<28; i++)
    {
        float fi = float(i);

        float radius = mix(0.04,0.20,hash(fi*1.13));
        float speed  = mix(0.35,1.10,hash(fi*2.71));
        float sway   = mix(0.02,0.10,hash(fi*3.31));

        // every light starts somewhere random and loops back around the bottom
        float x = (hash(fi*4.17)-0.5)*1.9*iResolution.x/iResolution.y;
        float y = fract(hash(fi*5.23) + t*speed)*1.6 - 0.8;
        x += sin(iTime*0.3*speed + fi)*sway;

        float d = length(uv-vec2(x,y));

        // an out of focus point light is a filled disc with a slightly brighter rim
        float disc = smoothstep(radius,radius*0.72,d);
        float rim  = smoothstep(radius,radius*0.93,d) - smoothstep(radius*0.93,radius*0.80,d);

        vec3 tint = 0.5 + 0.5*cos(vec3(0.0,2.1,4.2) + hash(fi*6.37)*6.2831);
        tint = mix(vec3(0.6,0.7,1.0),tint,0.55);

        col += tint*(disc*0.13 + rim*0.20);
    }

    vec2 v = (fragCoord/iResolution.xy)-0.5;
    col *= 1.0 - 0.75*dot(v,v);

    fragColor = vec4(col,1.0);
}
