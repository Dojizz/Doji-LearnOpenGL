#version 330 core
layout (location = 0) in vec3 aPos; // λ�ñ���������λ��ֵΪ0
layout (location = 1) in vec2 aTexCoords;
out vec4 vertexColor; // ΪƬ����ɫ��ָ��һ����ɫ���
out vec2 texCoords;


uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0); 
    texCoords = aTexCoords;
}