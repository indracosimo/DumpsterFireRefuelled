//#version 330 core
//
////matrices
//uniform mat4 modelMatrix = mat4(1);
//uniform mat4 viewMatrix = mat4(1);
//uniform mat4 projectionMatrix = mat4(1);;
//uniform mat4 normalMatrix = mat4(1);
////uniform mat4 camMatrix = mat4(1);
//
////Camera
//uniform vec3 eyePosition = vec3(0.0f, 0.0f, 3.0f);
//
////vertex atributes from vao
//layout (location = 0) in vec3 in_Position;      
//layout (location = 1) in vec2 in_UV; // use location 1 for texture coordinates
//// layout (location = 2) in vec3 aColor;
//layout (location = 2) in vec3 in_Normal;
//
////output to fragment shader
//out vec3 v_normal; 
//out vec2 UV_Coord;
//out vec3 position;
//out vec3 vecToEye;
//
//void main()
//{
//    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
//    gl_Position = mvp * vec4(in_Position, 1.0);
//    
//    //transform to world space
//    position = (modelMatrix * vec4(in_Position, 1.0)).xyz;
//    
//    v_normal = normalize(mat3(normalMatrix) * in_Normal);
//
//    UV_Coord = in_UV;
//    
//    vecToEye = normalize(eyePosition - position);
//
//}

#version 330 core

uniform mat4 modelMatrix = mat4(1);
uniform mat4 viewMatrix = mat4(1);
uniform mat4 projectionMatrix = mat4(1);
uniform mat4 normalMatrix = mat4(1);

uniform vec3 eyePosition = vec3(0.0f, 0.0f, 3.0f);

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_UV;
layout (location = 2) in vec3 in_Normal;

out vec3 v_normal;
out vec2 UV_Coord;
out vec3 fragPosition;
out vec3 vecToEye;

void main()
{
    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    
    gl_Position = mvp * vec4(in_Position, 1.0);

    fragPosition = vec3(modelMatrix * vec4(in_Position, 1.0));

    v_normal = normalize(mat3(normalMatrix) * in_Normal);

    UV_Coord = in_UV;
    vecToEye = normalize(eyePosition - fragPosition);
}