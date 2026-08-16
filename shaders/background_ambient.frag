// FlashySlideshows background : a heavily blurred , over-scaled copy of the photo you
// are currently looking at , so the background takes its colours from the album
// instead of being a fixed picture. The look most photo viewers and TV apps use.
//
// This one is not a generic ShaderToy shader , it is written against what
// visuals/dynamic_background.cpp feeds it :
//   iChannel0 / iChannelResolution[0] = the photo the camera is on
//   iChannel1 / iChannelResolution[1] = the photo it was on just before
//   iProgress                         = 0 -> 1 fade from the old one to the new one
// An unbound channel has iChannelResolution[n].x == 0 , which is how the fallbacks
// below know there is nothing to sample.

const int TAPS = 24;

// The photos are uploaded with row 0 at the top , and the 180 degree world roll is
// already cancelled out by the reversed texture coordinates on the photo quads , so on
// screen the only thing left to undo is that fragCoord.y counts upwards.
vec2 screenToTexture(vec2 uv)
{
    return vec2(uv.x,1.0-uv.y);
}

// "cover" fit : fill the window with the picture , keep its aspect ratio , crop the
// overhang evenly. overscale > 1 zooms further in , which hides the edges once the
// blur starts reaching outside the picture.
vec2 coverUV(vec2 uv,vec2 textureResolution,float overscale)
{
    float textureAspect = textureResolution.x / textureResolution.y;
    float viewAspect    = iResolution.x / iResolution.y;

    vec2 scale = vec2(1.0);
    if (textureAspect > viewAspect) { scale.x = viewAspect / textureAspect; }
    else                            { scale.y = textureAspect / viewAspect; }

    return (uv-0.5)*scale/overscale + 0.5;
}

// A disc of samples placed on a golden angle spiral , which gives an even coverage
// without needing a hand written kernel , weighted down towards the rim.
vec3 blurredSample(sampler2D picture,vec2 uv,float radius)
{
    vec3  accumulated = vec3(0.0);
    float totalWeight = 0.0;
    float aspect = iResolution.x / iResolution.y;

    for (int i=0; i<TAPS; i++)
    {
        float fi    = float(i);
        float angle = fi * 2.39996323;                 // golden angle , radians
        float r     = sqrt((fi+0.5)/float(TAPS));      // even spread over the disc

        vec2 offset = vec2(cos(angle),sin(angle))*r*radius;
        offset.y *= aspect;                            // keep the disc round on 16:9

        float weight = 1.0 - r*0.55;
        accumulated += texture(picture,clamp(uv+offset,0.0,1.0)).rgb * weight;
        totalWeight += weight;
    }
    return accumulated / totalWeight;
}

void mainImage(out vec4 fragColor,in vec2 fragCoord)
{
    vec2 uv = fragCoord / iResolution.xy;

    // A very slow drift , so a slideshow left on one picture does not look frozen
    vec2 drift = vec2(sin(iTime*0.05),cos(iTime*0.037))*0.012;

    vec3 col;

    if (iChannelResolution[0].x < 1.0)
    {
        // No photo is loaded yet ( still decoding , or the album is empty ) , fall
        // back to something that at least is not a black rectangle
        col = mix(vec3(0.03,0.04,0.07),vec3(0.08,0.07,0.12),uv.y);
    }
    else
    {
        vec2 uvNew = coverUV(screenToTexture(uv)+drift,iChannelResolution[0].xy,1.16);
        col = blurredSample(iChannel0,uvNew,0.055);

        if ( (iChannelResolution[1].x >= 1.0) && (iProgress < 1.0) )
        {
            vec2 uvOld = coverUV(screenToTexture(uv)+drift,iChannelResolution[1].xy,1.16);
            vec3 previous = blurredSample(iChannel1,uvOld,0.055);
            col = mix(previous,col,smoothstep(0.0,1.0,iProgress));
        }

        // Hold the background well under the photo that is drawn on top of it , and
        // pull the saturation down a little so it reads as a wash rather than a copy
        float luma = dot(col,vec3(0.2126,0.7152,0.0722));
        col = mix(vec3(luma),col,0.85);
        col *= 0.60;
    }

    vec2 v = uv-0.5;
    col *= 1.0 - 0.60*dot(v,v);

    // 8 bit gradients over a big blurred area band badly , this is under the noise floor
    float dither = fract(sin(dot(fragCoord,vec2(12.9898,78.233)))*43758.5453);
    col += (dither-0.5)/255.0;

    fragColor = vec4(col,1.0);
}
