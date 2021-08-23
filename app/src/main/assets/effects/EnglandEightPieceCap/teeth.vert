#version 300 es

precision highp sampler2DArray;

#define GLFX_LIGHTS
#define GLFX_TBN
#define GLFX_TEX_MRAO
#define GLFX_LIGHTING

layout( location = 0 ) in vec3 attrib_pos;
#ifdef GLFX_LIGHTING
layout( location = 1 ) in vec3 attrib_n;
#ifdef GLFX_TBN
layout( location = 2 ) in vec4 attrib_t;
#endif
#endif
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
    vec4 glfx_IDATA[48];
};
uniform uint glfx_CURRENT_I;
#define glfx_T_SPAWN (glfx_IDATA[glfx_CURRENT_I].x)
#define glfx_T_ANIM (glfx_IDATA[glfx_CURRENT_I].y)
#define glfx_ANIMKEY (glfx_IDATA[glfx_CURRENT_I].z)

uniform sampler2D glfx_BONES;

uniform sampler2D glfx_UVMORPH;

out vec2 var_uv;
#ifdef GLFX_LIGHTING
#ifdef GLFX_TBN
out vec3 var_t;
out vec3 var_b;
#endif
out vec3 var_n;
out vec3 var_v;
#endif

#ifdef GLFX_USE_SHADOW
out vec3 var_shadow_coord;
vec3 spherical_proj( vec2 fovM, vec2 fovP, float zn, float zf, vec3 v )
{
    vec2 xy = (atan( v.xy, v.zz )-(fovP+fovM)*0.5)/((fovP-fovM)*0.5);
    float z = (length(v)-(zn+zf)*0.5)/((zf-zn)*0.5);
    return vec3( xy, z );
}
#endif

mat3x4 get_bone( uint bone_idx, int y, float t )
{
    ivec2 p = ivec2(int(bone_idx)*3,y);

    mat3x4 m = (1.-t)*mat3x4( 
        texelFetch( glfx_BONES, p, 0 ),
        texelFetchOffset( glfx_BONES, p, 0, ivec2(1,0) ),
        texelFetchOffset( glfx_BONES, p, 0, ivec2(2,0) ) );
    
    m += t*mat3x4( 
        texelFetchOffset( glfx_BONES, p, 0, ivec2(0,1) ),
        texelFetchOffset( glfx_BONES, p, 0, ivec2(1,1) ),
        texelFetchOffset( glfx_BONES, p, 0, ivec2(2,1) ) );
    
    return m;
}

mat3x4 get_transform()
{
    int y = int(glfx_ANIMKEY);
    float t = fract(glfx_ANIMKEY);
    mat3x4 m = get_bone( attrib_bones[0], y, t );
#ifndef GLFX_1_BONE
    if( attrib_weights[1] > 0. )
    {
        m = m*attrib_weights[0] + get_bone( attrib_bones[1], y, t )*attrib_weights[1];
        if( attrib_weights[2] > 0. )
        {
            m += get_bone( attrib_bones[2], y, t )*attrib_weights[2];
            if( attrib_weights[3] > 0. )
                m += get_bone( attrib_bones[3], y, t )*attrib_weights[3];
        }
    }
#endif

    return m;
}

void main()
{
    mat3x4 m = get_transform();

    vec3 vpos = attrib_pos;
    vpos = vec4(vpos,1.)*m;

    vec2 flip_uv = vec2( 0.5, 0.33 );
    vec3 translation = texture(glfx_UVMORPH,flip_uv).xyz;
    vpos += translation;

    gl_Position = glfx_MVP * vec4(vpos,1.);

    var_uv = attrib_uv;

#ifdef GLFX_LIGHTING
    var_n = mat3(glfx_MV)*(attrib_n*mat3(m));
#ifdef GLFX_TBN
    var_t = mat3(glfx_MV)*(attrib_t.xyz*mat3(m));
    var_b = attrib_t.w*cross( var_n, var_t );
#endif
    var_v = (glfx_MV*vec4(vpos,1.)).xyz;
#endif
}