#version 450 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

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

in vec3 fragPos;
in vec3 color;
in vec3 normal;
in vec2 texCoord;
in float ID;

uniform sampler2DArray texture1;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
       /*float distance = length(light.position - fragPos);
       float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

       vec3 ambient = light.ambient * texture(texture1, vec3(texCoord.x,texCoord.y,int(ID))).rgb;

       // diffuse
       vec3 norm = normalize(normal);
       vec3 lightDir = normalize(light.position - fragPos);
       float diff = max(dot(norm, lightDir), 0.0);
       vec3 diffuse = light.diffuse * diff * texture(texture1, vec3(texCoord.x,texCoord.y,int(ID))).rgb;
       diffuse *= attenuation;

       vec3 result = ambient + diffuse;
       FragColor = vec4(result, 1.0);*/

       gPosition = fragPos;
           // also store the per-fragment normals into the gbuffer
       gNormal = normalize(Normal);
           // and the diffuse per-fragment color
       gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
}