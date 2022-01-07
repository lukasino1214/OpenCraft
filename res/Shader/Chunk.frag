/*#version 450 core
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
}*/

#version 450 core
out vec4 FragColor;

struct Material {
    sampler2DArray textures;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 color;
in vec3 normal;
in vec2 texCoord;
in float ID;

uniform sampler2DArray texture1;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
    //FragColor = texture(texture1, vec3(texCoord.x,texCoord.y,int(ID)));

        float distance = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        vec3 ambient = light.ambient * texture(texture1, vec3(texCoord.x,texCoord.y,int(ID))).rgb;

        // diffuse
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(light.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(texture1, vec3(texCoord.x,texCoord.y,int(ID))).rgb;
        //ambient *= attenuation;
        diffuse *= attenuation;

        vec3 result = ambient + diffuse;
        FragColor = vec4(result, 1.0);
}