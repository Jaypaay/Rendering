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
    float r = texture_color.rgba.r;
    float g = texture_color.rgba.g;
    float b = texture_color.rgba.b;
    float a = texture_color.rgba.a;

    vec3 textureColorWithoutAlpha = vec3(texture_color.rgba.r,texture_color.rgba.g,texture_color.rgba.b);
    float intensity = dot(normalize(textureColorWithoutAlpha), normalize(light_direction)) + 0.3;
    out_color = vec4(textureColorWithoutAlpha * -intensity, texture_color.rgba.a) ;
    //Utilisé avant pour afficher lesnormals du bateau
    //out_color = vec4(normals, 1);
    //out_color = vec4(texture_color.x, texture_color.y, 0., 1.);
    //UV
    
    //out_color = vec4(vertex_position, 1.);
    //out_color = vec4(0.84, 0.23, 0.23, 1.0);
}