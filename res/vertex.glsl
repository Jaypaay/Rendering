#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normals;
uniform float aspect_ratio;
uniform float movement;

// 3D exercice
uniform mat4 matrix;
out vec3 vertex_position;
out vec2 uv;
out vec3 normals;

void main()
{
    uv = in_uv;
    normals = in_normals;
    vec3 position = in_position;
    vertex_position = in_position;
    // Was used to add an offset to the rectangle
    //position.x = in_position.x + 0.4f;
    //position.y = in_position.y + 0.4f;
    position.x = position.x / aspect_ratio;
    position.y = position.y / aspect_ratio;
    position.z = position.z / aspect_ratio;
    //position.x += movement/10;
    gl_Position = matrix * vec4(position, 1.);
    //gl_Position = vec4(position , 0.0f ,1.0f);
}