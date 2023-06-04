#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float a_TexIndex;

uniform mat4 ul_Projection;
uniform mat4 ul_View;
uniform mat4 ul_Model;

out vec2 v_TexCoord;
out float v_TexIndex;

void main() {
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	gl_Position = ul_Projection * ul_View * (ul_Model * vec4(a_Position, 1.0));
};

////////////////////////////////////////////////////////////////////////////////
#shader fragment
#version 410 core

in vec2 v_TexCoord;
in float v_TexIndex;

struct Material {
	sampler2D diffusion[16];
	sampler2D specular[16];
	float shininess;
};

uniform Material u_Material;

uniform vec3 u_LampColor;

out vec4 color;

void main() {
	int index = int(v_TexIndex);
	vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
	vec4 texColor = texture(u_Material.diffusion[index], v_TexCoord);

	vec4 output = texColor * ambient;

	output += texColor * vec4(u_LampColor, 1.0) * u_Material.shininess;

	color = output;
};