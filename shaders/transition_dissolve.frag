// FlashySlideshows transition : a grainy dissolve , pixels cross over in an order
// decided by noise rather than all at once.
// GL Transitions dialect , see shaders/transition_crossfade.frag.

const float softness  = 0.18;
const float grainSize = 220.0;

float hash(vec2 p)
{
    return fract(sin(dot(p,vec2(127.1,311.7)))*43758.5453123);
}

vec4 transition(vec2 uv)
{
    // Quantize first , so the dissolve shows visible grains instead of per pixel static
    float threshold = hash(floor(uv*grainSize)/grainSize);

    // The sweep runs from -softness to 1+softness rather than 0 to 1 , so the grain
    // that drew threshold 0 and the one that drew threshold 1 both get to finish
    float sweep = progress*(1.0+2.0*softness) - softness;
    float t = smoothstep(threshold-softness,threshold+softness,sweep);

    return mix(getFromColor(uv),getToColor(uv),t);
}
