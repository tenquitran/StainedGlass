#version 440 core
#pragma debug(on)
#pragma optimize(off)

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

layout(location = 0) uniform mat4 MVP;
layout(location = 1) uniform mat4 Model;
layout(location = 2) uniform mat4 ModelView;
layout(location = 3) uniform mat3 Normal;
//layout(location = 4) uniform mat4 Projection;
//layout(location = 5) uniform vec3 WorldCameraPosition;
layout(location = 4) uniform mat4 ProjectorMatrix;

out vec3 vPos;
out vec3 vNormal;

out vec3 eyeNormal;       // Normal in eye coordinates
out vec4 eyePosition;     // Position in eye coordinates
out vec4 projTexCoord;

void main()
{
	vNormal = normalize(Normal * normal);

	vPos = vec3(ModelView * position);

	eyeNormal = normalize(Normal * normal);
    eyePosition = ModelView * position;

    projTexCoord = ProjectorMatrix * position;
    //projTexCoord = ProjectorMatrix * (Model * position);

	gl_Position = MVP * position;
}
