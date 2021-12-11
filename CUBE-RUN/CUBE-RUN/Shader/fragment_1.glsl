#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 fragLightSpace;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;


uniform sampler2D shadowMap;
uniform sampler2D texture0;

out vec4 FragColor;

float shadowCalculate(vec4 fragPos, float dotNormal)
{
	vec3 ndc = (fragPos.xyz / fragPos.w) * 0.5 + 0.5;
	if (ndc.z > 1.0) ndc.z = 1.0;

	float shadow = 0.0;

	float bias = max(0.001 * (1.0 - dotNormal), 0.0008);

	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			float depth = texture(shadowMap, ndc.xy + vec2(i, j) * (1.0 / textureSize(shadowMap, 0))).r;
			shadow += (depth + bias) < ndc.z ? 0.0 : 1.0;
		}
	}

	return shadow / 9.0;
}

void main(void)
{
	// ambient
	vec3 ambient = lightColor * 0.3;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float dotNormal = dot(norm, lightDir);
	float diff = max(dotNormal, 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	int shininess = 64;
	float howSpecular = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = howSpecular * spec * lightColor;  

	// shadow
	float shadow = shadowCalculate(fragLightSpace, dotNormal);

	// result
	vec3 result = (ambient + shadow * (specular + diffuse));
	FragColor = texture(texture0, TexCoord) * vec4(result, 1.0);
}