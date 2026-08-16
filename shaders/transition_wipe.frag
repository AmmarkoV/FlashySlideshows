// FlashySlideshows transition : a soft edged diagonal wipe.
// GL Transitions dialect , see shaders/transition_crossfade.frag.

const float softness = 0.12;
const vec2  direction = vec2(1.0,0.35);

vec4 transition(vec2 uv)
{
    vec2 d = normalize(direction);

    // How far along the wipe direction this pixel sits , normalized to 0..1 so the
    // sweep covers the whole frame whatever the angle is
    float extent = abs(d.x) + abs(d.y);
    float here   = (dot(uv-0.5,d) + 0.5*extent) / extent;

    // The leading edge has to travel a little past both ends to account for the
    // softness , otherwise the last sliver never finishes
    float edge = progress*(1.0+2.0*softness) - softness;
    float t    = smoothstep(edge-softness,edge+softness,here);

    return mix(getToColor(uv),getFromColor(uv),t);
}
