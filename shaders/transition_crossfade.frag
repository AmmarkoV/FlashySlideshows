// FlashySlideshows transition : the plain dissolve , with a slight ease so it does not
// start and stop abruptly.
//
// Written in the GL Transitions dialect ( gl-transitions.com ) : no mainImage , just a
// vec4 transition(vec2 uv) that reads progress and calls getFromColor / getToColor.
// visuals/shadertoy.cpp notices there is no mainImage and provides those , letting any
// shader from that collection be dropped into this directory unmodified.

vec4 transition(vec2 uv)
{
    float t = smoothstep(0.0,1.0,progress);
    return mix(getFromColor(uv),getToColor(uv),t);
}
