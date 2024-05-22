#version 410

layout(location = 0) in vec2 in_position;

void main()
{
    vec2 position = in_position;
    
    position.x = in_position.x + 0.4f;
    position.y = in_position.y + 0.4f;
    gl_Position = vec4(position , 0.0f ,1.0f);
}