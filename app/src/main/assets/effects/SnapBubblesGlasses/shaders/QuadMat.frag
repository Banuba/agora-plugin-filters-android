#include <bnb/glsl.frag>


// in vec3 var_uvw;
BNB_IN(0) vec2 var_uv;


BNB_DECLARE_SAMPLER_VIDEO(0, 1, glfx_VIDEO);

void main()
{
	bnb_FragColor = BNB_TEXTURE_2D(BNB_SAMPLER_2D(glfx_VIDEO), var_uv) * 0.6;
}
