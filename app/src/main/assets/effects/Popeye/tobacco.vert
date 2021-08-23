#version 300 es

#define GLFX_USE_AUTOMORPH
//#define GLFX_1_BONE

layout( location = 0 ) in vec3 attrib_pos;
layout( location = 1 ) in vec3 attrib_n;
layout( location = 2 ) in vec4 attrib_t;
layout( location = 3 ) in vec2 attrib_uv;
layout( location = 4 ) in uvec4 attrib_bones;
#ifndef GLFX_1_BONE
layout( location = 5 ) in vec4 attrib_weights;
#endif

layout(std140) uniform glfx_GLOBAL
{
	mat4 glfx_MVP;
	mat4 glfx_PROJ;
	mat4 glfx_MV;
};
layout(std140) uniform glfx_INSTANCES
{
	vec4 glfx_IDATA[8];
};
uniform uint glfx_CURRENT_I;
#define glfx_T_SPAWN (glfx_IDATA[glfx_CURRENT_I].x)
#define glfx_T_ANIM (glfx_IDATA[glfx_CURRENT_I].y)
#define glfx_ANIMKEY (glfx_IDATA[glfx_CURRENT_I].z)

uniform sampler2D glfx_BONES;

#ifdef GLFX_USE_AUTOMORPH
uniform sampler2D glfx_MORPH;
vec2 glfx_morph_coord( vec3 v )
{
	const float half_angle = radians(104.);
	const float y0 = -210.;
	const float y1 = 1612.;
	float x = atan( v.x, v.z )/half_angle;
	float y = ((v.y-y0)/(y1-y0))*2. - 1.;
	return vec2(x,y);
}
vec3 glfx_auto_morph( vec3 v )
{
	vec2 morph_uv = glfx_morph_coord(v)*0.5 + 0.5;
	vec3 translation = texture( glfx_MORPH, morph_uv ).xyz;
	return v + translation;
}
#endif

out vec2 var_uv;
out vec3 var_t;
out vec3 var_b;
out vec3 var_n;
out vec3 var_v;

mat3x4 get_bone( uint bone_idx, float k )
{
	float bx = float( int(bone_idx)*3 );
	vec2 rts = 1./vec2(textureSize(glfx_BONES,0));
	return mat3x4( 
		texture( glfx_BONES, (vec2(bx,k)+0.5)*rts ),
		texture( glfx_BONES, (vec2(bx+1.,k)+0.5)*rts ),
		texture( glfx_BONES, (vec2(bx+2.,k)+0.5)*rts ) );
}

void main()
{
	mat3x4 m = get_bone( attrib_bones[0], glfx_ANIMKEY );
#ifndef GLFX_1_BONE
	if( attrib_weights[1] > 0. )
	{
		m = m*attrib_weights[0] + get_bone( attrib_bones[1], glfx_ANIMKEY )*attrib_weights[1];
		if( attrib_weights[2] > 0. )
		{
			m += get_bone( attrib_bones[2], glfx_ANIMKEY )*attrib_weights[2];
			if( attrib_weights[3] > 0. )
				m += get_bone( attrib_bones[3], glfx_ANIMKEY )*attrib_weights[3];
		}
	}
#endif

	vec3 vpos = vec4(attrib_pos,1.)*m;

#ifdef GLFX_USE_AUTOMORPH
	vpos = glfx_auto_morph( vpos );
#endif

	gl_Position = glfx_MVP * vec4(vpos,1.);

	var_uv = attrib_uv;
    
    var_v = (glfx_MV*vec4(vpos,1.)).xyz;
    
	var_t = mat3(glfx_MV)*(attrib_t.xyz*mat3(m));
	var_n = mat3(glfx_MV)*(attrib_n*mat3(m));
	var_b = attrib_t.w * cross( var_n, var_t );
}
