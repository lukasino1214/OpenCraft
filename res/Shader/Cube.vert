#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in float aID;

out vec3 color;
out vec2 texCoord;
out float ID;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    color = aColor;
    texCoord = vec2(aTexCoord.x, aTexCoord.y);
    ID = aID;
}