#version 410

layout(location = 0) in vec2 in_position;
uniform float aspect_ratio;
uniform float movement;

// 3D exercice
uniform mat4 matrix;

void main()
{
    vec2 position = in_position;

    // Was used to add an offset to the rectangle
    //position.x = in_position.x + 0.4f;
    //position.y = in_position.y + 0.4f;
    position.x = position.x / aspect_ratio;
    position.x += movement/10;
    
    gl_Position = matrix * vec4(position, 0., 1.);
    //gl_Position = vec4(position , 0.0f ,1.0f);
}