#version 440 core
#pragma debug(on)
#pragma optimize(off)

struct LightInfo
{
	// The first 3 fields are the intensities of the light components.
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec4 position;
};
uniform LightInfo Light;

struct MaterialInfo
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform MaterialInfo Material;

in vec3 vPos;
in vec3 vNormal;

in vec3 eyeNormal;       // Normal in eye coordinates
in vec4 eyePosition;     // Position in eye coordinates
in vec4 projTexCoord;

layout(binding = 0) uniform sampler2D glassTexture;

out vec4 outColor;

void main()
{
	vec3 n = normalize(vNormal);
	vec3 s = normalize(vec3(Light.position) - vPos);
	vec3 v = normalize(vec3(-vPos));
	vec3 r = reflect(-s, n);

	vec3 ambient  = Light.ambient * Material.ambient;
	vec3 diffuse  = Light.diffuse * Material.diffuse * max(dot(s, n), 0.0);
	vec3 specular = Light.specular * Material.specular * pow(max(dot(r, v), 0.0), Material.shininess);

	vec4 phongColor = vec4(ambient + diffuse + specular, 1.0);

	vec4 projTexColor = vec4(0.0);
    if (projTexCoord.z > 0.0)
	{
        projTexColor = textureProj(glassTexture, projTexCoord);

		outColor = mix(phongColor, projTexColor, 0.9);
		//outColor = phongColor + projTexColor * 0.5;
	}
	else
	{
		outColor = phongColor;
	}
}
