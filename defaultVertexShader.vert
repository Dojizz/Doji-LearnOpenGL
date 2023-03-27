#version 330 core
layout (location = 0) in vec3 aPos; // 位置变量的属性位置值为0
layout (location = 1) in vec2 aTexCoords;
out vec4 vertexColor; // 为片段着色器指定一个颜色输出
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