#version 450 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in float ID;

uniform sampler2DArray texture1;

void main() {
    //FragColor = vec4(color, 1.0f);
    //FragColor = vec4(vec3(1.0/ID, 1.0/ID, 1.0/ID), 1.0f);
    //FragColor = texture(texture1, texCoord);
    //FragColor = mix(texture(texture1, texCoord), vec4(color, 1.0f), 0.2);
    FragColor = texture(texture1, vec3(texCoord.x,texCoord.y,int(ID)));
}