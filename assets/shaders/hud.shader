#version 410 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIndex;

uniform mat4 orthoMat;

out float index;
out vec2 v_TexCoord;

void main() {
	index = texIndex;
	v_TexCoord = texCoord;
	gl_Position = orthoMat * position;
};

#shader fragment
#version 410 core

in vec2 v_TexCoord;
in float index;

uniform sampler2D u_Texture[32];

out vec4 color;

void main() {
	int i = int(index);
	vec4 texColor = texture(u_Texture[i], v_TexCoord);
	color = texColor;
};