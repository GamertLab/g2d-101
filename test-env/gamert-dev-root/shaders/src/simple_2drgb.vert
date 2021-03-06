#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform u_stable_info
{
	vec2	extent;
} stable_info;

layout(set = 0, binding = 1) uniform u_combined_drawcall_info
{
	mat3	view;
} combined_drawcall_info;

layout(set = 1, binding = 0) uniform u_single_drawcall_info
{
	mat3	world;
} single_drawcall_info;

layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec3 in_clr;

layout(location = 0) out vec3 out_clr;

void main()
{
	vec3 real_pos = vec3(in_pos.x, in_pos.y, 1.0);
	real_pos = single_drawcall_info.world * real_pos;
	real_pos.x /= stable_info.extent.x;
	real_pos.y /= stable_info.extent.y;
    gl_Position = vec4(real_pos.x, real_pos.y, 0.0, 1.0);
    out_clr = in_clr;
}