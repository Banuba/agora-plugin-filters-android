#version 300 es

precision highp float;
precision highp sampler2DArray;

// in vec3 var_uvw;
in vec2 var_uv;

uniform sampler2D glfx_VIDEO;
layout( location = 0 ) out vec4 frag_color;

// uniform sampler2DArray animation;
uniform sampler2D meshTexture;
void main()
{
	frag_color = texture(glfx_VIDEO, var_uv) * 0.6;
}
