#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    // gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);
    // gl_Position = transform * vec4(aPos, 1.0f);
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    FragPos = vec3(model * vec4(aPos, 1.0));
    // Normal = aNormal;
    // Normal = mat3(transpose(inverse(model))) * aNormal;

    // ourColor = aColor;
    TexCoord = aTexCoord;
}
