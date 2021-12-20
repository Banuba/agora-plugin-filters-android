#include <bnb/glsl.frag>

BNB_IN(0) vec2 var_uv;

BNB_DECLARE_SAMPLER_2D(0, 1, tex_src);
BNB_DECLARE_SAMPLER_2D(2, 3, tex_bg);


float blendOverlay(float base, float blend) {
	return base<0.5?(2.0*base*blend):(1.0-2.0*(1.0-base)*(1.0-blend));
}

vec3 blendOverlay(vec3 base, vec3 blend) {
	return vec3(blendOverlay(base.r,blend.r),blendOverlay(base.g,blend.g),blendOverlay(base.b,blend.b));
}

vec3 blendOverlay(vec3 base, vec3 blend, float opacity) {
	return (blendOverlay(base, blend) * opacity + base * (1.0 - opacity));
}

vec3 blendHardLight(vec3 base, vec3 blend) {
	return blendOverlay(blend,base);
}

vec3 blendHardLight(vec3 base, vec3 blend, float opacity) {
	return (blendHardLight(base, blend) * opacity + base * (1.0 - opacity));
}

void main()
{   
    vec2 uv = var_uv;    
    vec4 bg = BNB_TEXTURE_2D(BNB_SAMPLER_2D(tex_bg), uv);
    vec4 color = BNB_TEXTURE_2D(BNB_SAMPLER_2D(tex_src), uv);

    color.rgb = blendHardLight(color.rgb,bg.rgb,0.7);
    
    bnb_FragColor = color;
}