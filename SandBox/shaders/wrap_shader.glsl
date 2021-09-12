#shader vertex
#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3, component = 0) in float tex_index;
layout(location = 3, component = 1) in float material_id;
layout(location = 4) in vec3 normal;

layout(binding = 0) buffer GlobalMatrices 
{
    mat4 proj_view;
};

out flat vec4 out_color;
out vec2 out_tex_coord;
out flat float out_tex_index;
out vec4 out_pos;

void main()
{
	gl_Position = proj_view * vec4(pos, 1.0);
	out_color = color;
	out_tex_coord = tex_coord;
	out_tex_index = tex_index;
	out_pos = vec4(pos, 1.0);
}

#shader fragment
#version 450 core

out vec4 frag_color;

in flat vec4 out_color;
in vec2 out_tex_coord;
in flat float out_tex_index;
in vec4 out_pos;

uniform sampler2D textures[32];

uniform float screen_width;
uniform float screen_height;

/*
void wrap_coordinate(float x, float y, float* ox, float* oy) {
	*ox = x;
	*oy = y;

	if (x < 0.0)
		*ox = x + screen_width;
	if (x >= screen_width)
		*ox = x - screen_width;
	if (y < 0.0)
		*oy = y + screen_height;
	if (y >= screen_height)
		*oy = y - screen_height;
}
*/

void main()
{
	vec2 st = gl_FragCoord.xy/vec2(screen_width, screen_height);
	
	frag_color = vec4(st.x, st.y, st.x, 1.0);
}