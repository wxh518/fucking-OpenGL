#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 v_TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_TextureWall;
uniform sampler2D u_TextureFace;

void main()
{
	vec4 texColor = mix(texture(u_TextureWall, v_TexCoord), texture(u_TextureFace, v_TexCoord), 0.2);
	color = texColor;
};