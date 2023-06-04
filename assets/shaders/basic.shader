#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 vertexcolor;
layout(location = 4) in float texIndex;

//Texture Pass to Fragment
out vec2 v_TexCoord;
out float v_TexIndex;

//Final Fragment Color
out vec3 finalcolor;

//Normal and Fragment Position for Lighting
out vec3 v_Normal;
out vec3 v_FragPos;
//out vec4 v_LightSpacePosition[50];

//MVP Matrix
uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
//uniform mat4 u_LightSpace[50];

uniform mat3 u_Normal;

void main() {
	//
	v_TexCoord = texCoord;
	v_TexIndex = texIndex;
	//
	finalcolor = vertexcolor;
	//
	v_Normal = normalize(u_Normal * normal);
	v_FragPos = vec3(u_Model * vec4(position, 1.0));
	//
	for (int i = 0; i < 50; i++) {
		//v_LightSpacePosition[i] = u_LightSpace[i] * (u_Model * vec4(position, 1.0));
	}
	//
	gl_Position = u_Projection * u_View * (u_Model * vec4(position, 1.0));
};


////////////////////////////////////////////////////////////////////////////
#shader fragment
#version 410 core

//Input Variables

in vec2 v_TexCoord;
in float v_TexIndex;

in vec3 finalcolor;

in vec3 v_Normal;
in vec3 v_FragPos;

//in vec4 v_LightSpacePosition[50];

out vec4 output;

//Material Struct

struct Material {
	sampler2D diffusion[16];
	sampler2D specular[16];
	float shininess;
};

uniform Material u_Material;

//Light Struct

struct dirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffusion;

	float brightness;
};

struct pointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffusion;

	float Kc;
	float Kl;
	float Kq;

	float brightness;
};

struct spotLight {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffusion;

	float Kc;
	float Kl;
	float Kq;

	float cutOff;
	float outerCutOff;

	float brightness;
};

//Light Uniforms
uniform vec3 u_ViewPos;

#define NR_PT_LIGHTS 50
#define NR_ST_LIGHTS 50

uniform dirLight u_dLight;
uniform pointLight u_pLight[NR_PT_LIGHTS];
uniform spotLight u_sLight[NR_ST_LIGHTS];

uniform sampler2D u_Flashlight;

//Function Declarations
vec4 getAmbience(const vec4 p_diffuseMap);
vec4 getDiffusion(const vec3 lightDiffusion, const vec4 p_diffuseMap, const vec3 p_lightDir);
vec4 getSpecular(const vec4 p_specularMap, const vec3 p_lightDir);

vec4 getDirLight(const vec4 p_diffuseMap, const vec4 p_specularMap);
vec4 getPointLight(const pointLight light, const vec4 p_diffuseMap, const vec4 p_specularMap);
vec4 getSpotLight(const spotLight light, const vec4 p_diffuseMap, const vec4 p_specularMap, const int i);

void main() {	
	//Getting Texture Fragment Color
	int index = int(v_TexIndex);
	vec4 diffuseMap = texture(u_Material.diffusion[index], v_TexCoord);
	vec4 specularMap = texture(u_Material.specular[index], v_TexCoord);

	//Getting Directional Light
	vec4 color = getDirLight(diffuseMap, specularMap);

	//Getting Point Lights
	for (int i = 0; i < NR_PT_LIGHTS; i += 1) {
		if (u_pLight[i].brightness != 0.0) {
			color += getPointLight(u_pLight[i], diffuseMap, specularMap);
		}
	}

	//Getting Spotlight
	for (int i = 0; i < NR_ST_LIGHTS; i += 1) {
		if (u_sLight[i].brightness != 0.0) {
			color += getSpotLight(u_sLight[i], diffuseMap, specularMap, i);
		}
	}

	if (color.a < 0.1) {
		discard;
	}

	output = color;
};

//Function Definitions

vec4 getAmbience(const vec3 lightAmbient, const vec4 p_diffuseMap) {
	return (vec4(lightAmbient, 1.0) * p_diffuseMap);
};

vec4 getDiffusion(const vec3 lightDiffusion, const vec4 p_diffuseMap, const vec3 p_lightDir) {
	float diffusionStrength = max(dot(v_Normal, p_lightDir), 0.0);

	return (vec4(lightDiffusion, 1.0) * (diffusionStrength * p_diffuseMap));
};

vec4 getSpecular(const vec3 lightSpecular, const vec4 p_specularMap, const vec3 p_lightDir) {
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-p_lightDir, v_Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

	return (vec4(lightSpecular, 1.0) * (spec * p_specularMap));
};

vec4 getDirLight(const vec4 p_diffuseMap, const vec4 p_specularMap) {
	//Light Direction
	vec3 lightDir = normalize(u_dLight.direction);

	//Diffusion
	vec4 diffuse = getDiffusion(u_dLight.diffusion, p_diffuseMap, lightDir);

	//Specular
	vec4 specular =  getSpecular(u_dLight.diffusion, p_specularMap, lightDir);

	//Ambient
	vec4 ambient = getAmbience(u_dLight.ambient, p_diffuseMap);

	return ((ambient + diffuse + specular) * u_dLight.brightness);
};

vec4 getPointLight(const pointLight light, const vec4 p_diffuseMap, const vec4 p_specularMap) {
	//Light Direction
	vec3 lightDir = normalize(light.position - v_FragPos);

	//Attenuation
	float distance = length(light.position - v_FragPos);
	float att = 1.0 / (light.Kc + light.Kl * distance + light.Kq * (distance * distance));

	//Diffusion
	vec4 diffuse = att * getDiffusion(light.diffusion, p_diffuseMap, lightDir);

	//Specular
	vec4 specular = att * getSpecular(light.diffusion, p_specularMap, lightDir);

	//Ambient
	vec4 ambient = att * getAmbience(light.ambient, p_diffuseMap);

	return ((ambient + diffuse + specular) * light.brightness);
};

vec4 getSpotLight(const spotLight light, const vec4 p_diffuseMap, const vec4 p_specularMap, const int i) {
	//Light Direction
	vec3 lightDir = normalize(light.position - v_FragPos);

	//Attenuation
	float distance = length(light.position - v_FragPos);
	float att = 1.0 / (light.Kc + light.Kl * distance + light.Kq * (distance * distance));

	//Intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	//vec4 cookie = texture(u_Flashlight, (vec2(v_LightSpacePosition[i]) / -v_LightSpacePosition[i].z) + vec2(0.5));

	//Diffusion
	vec4 diffuse = intensity * att  * getDiffusion(light.diffusion, p_diffuseMap, lightDir);

	//Specular
	vec4 specular = intensity * att  * getSpecular(light.diffusion, p_specularMap, lightDir);

	//Ambient
	vec4 ambient = att * getAmbience(light.ambient, p_diffuseMap);

	return ((ambient + diffuse + specular) * light.brightness);
};