#include <bnb/glsl.frag>
#include <bnb/transform_uv.glsl>

#define Y_OFFSET 0.3
#define Y_SCALE 1. - Y_OFFSET
#define X_OFFSET 0.1
BNB_IN(0) vec2 var_uv;
BNB_IN(1) vec2 var_bgmask_uv;
BNB_IN(2) vec2 video_uv;



BNB_DECLARE_SAMPLER_2D(2, 3, glfx_BACKGROUND);

BNB_DECLARE_SAMPLER_VIDEO(4, 5, glfx_VIDEO);

BNB_DECLARE_SAMPLER_2D(0, 1, glfx_BG_MASK);

float filtered_bg_simple( BNB_DECLARE_SAMPLER_2D_ARGUMENT(mask_tex), vec2 uv )
{
	float bg1 = BNB_TEXTURE_2D(BNB_SAMPLER_2D(mask_tex), uv ).x;
	if( bg1 > 0.98 || bg1 < 0.02 )
		return bg1;

	vec2 o = 1./vec2(textureSize(BNB_SAMPLER_2D(mask_tex),0));
	float bg2 = BNB_TEXTURE_2D(BNB_SAMPLER_2D(mask_tex), uv + vec2(o.x,0.) ).x;


	return 0.2*(bg1+bg2);
}

void main()
{	
  vec2 uv = var_uv;
  vec4 bg = BNB_TEXTURE_2D(BNB_SAMPLER_2D(glfx_BACKGROUND), var_uv);


	uv = bnb_contain_uv (uv,vec2(480.,864.),1.,1.);
  vec4 bg_video = BNB_TEXTURE_2D(BNB_SAMPLER_2D(glfx_VIDEO), uv);
  vec4 blend_color_2 = (1. - (1. - bg) * (1. - bg_video));

  float mask = filtered_bg_simple( BNB_PASS_SAMPLER_ARGUMENT(glfx_BG_MASK), var_bgmask_uv );
  bnb_FragColor = mix(bg, blend_color_2, mask * 0.7);
	// bnb_FragColor = vec4(blend_color_2, mask);
}