// FlashySlideshows transition : the outgoing photo rushes towards the viewer and
// smears , the incoming one settles back out of the same smear.
//
// This one is written in the ShaderToy dialect instead ( it has a mainImage ) , to keep
// both paths through visuals/shadertoy.cpp exercised. It has to do its own letterboxing
// because getFromColor / getToColor only exist in the GL Transitions dialect.

const int SAMPLES = 12;

vec4 fitted(sampler2D img,vec3 res,vec2 uv)
{
    // Transparent , not black : transitions are drawn with blending on so that the
    // animated background keeps showing wherever the photo is not
    if (res.x<1.0) { return vec4(0.0); }

    float pictureAspect = res.x/res.y;
    float viewAspect    = iResolution.x/iResolution.y;

    vec2 scale = vec2(1.0);
    if (pictureAspect>viewAspect) { scale.y = pictureAspect/viewAspect; }
    else                          { scale.x = viewAspect/pictureAspect; }

    vec2 p = (uv-0.5)*scale + 0.5;
    if ( (p.x<0.0)||(p.x>1.0)||(p.y<0.0)||(p.y>1.0) ) { return vec4(0.0); }

    // photos are stored with row 0 at the top , uv here has y going up
    return texture(img,vec2(p.x,1.0-p.y));
}

// Average a short trail of samples along the line from the centre to this pixel ,
// which is what a zoom blur is
vec4 zoomBlur(sampler2D img,vec3 res,vec2 uv,float strength)
{
    vec4 accumulated = vec4(0.0);
    vec2 toCentre = uv-0.5;

    for (int i=0; i<SAMPLES; i++)
    {
        float k = float(i)/float(SAMPLES-1);
        accumulated += fitted(img,res,0.5 + toCentre*(1.0 - strength*k));
    }
    return accumulated/float(SAMPLES);
}

void mainImage(out vec4 fragColor,in vec2 fragCoord)
{
    vec2 uv = fragCoord/iResolution.xy;

    // Both halves peak in the middle of the transition and are sharp at their own end
    float outgoing = smoothstep(0.0,1.0,iProgress);
    float rush     = sin(iProgress*3.14159265);

    vec4 from = zoomBlur(iChannel0,iChannelResolution[0],uv, 0.28*rush);
    vec4 to   = zoomBlur(iChannel1,iChannelResolution[1],uv,-0.28*rush);

    fragColor = mix(from,to,outgoing);
}
