#shader vertex
#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3, component = 0) in float tex_index;
layout(location = 3, component = 1) in float material_id;

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

float plot(vec2 st, float pct){
  return  smoothstep( pct-0.02, pct, st.y) -
          smoothstep( pct, pct+0.02, st.y);
}

void main()
{
	vec2 st = gl_FragCoord.xy/vec2(1280.0, 720.0);

    float y = sqrt(st.x - 0.5) + 0.5;
    vec3 color = vec3(y);

    // Plot a line
    float pct = plot(st, y);
    color = (1.0-pct)*color+pct*vec3(0.0,1.0,0.0);

	gl_FragColor = vec4(color,1.0);
}