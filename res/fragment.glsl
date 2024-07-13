#version 410

out vec4 out_color;
in vec3 vertex_position;
in vec2 uv;
uniform sampler2D sampler;
uniform vec3 light_direction;
in vec3 normals;

void main()
{
    //Light intensity
    vec4 texture_color = texture(sampler, uv);
    float intensity = dot(normalize(normals), normalize(light_direction)) + 0.3;
    out_color = texture_color * -intensity;
    //Utilisé avant pour afficher lesnormals du bateau
    //out_color = vec4(normals, 1);
    //out_color = vec4(texture_color.x, texture_color.y, 0., 1.);
    //UV
    
    //out_color = vec4(vertex_position, 1.);
    //out_color = vec4(0.84, 0.23, 0.23, 1.0);
}