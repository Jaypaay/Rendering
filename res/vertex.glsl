#version 410

layout(location = 0) in vec2 pos;
uniform float aspectRatio;
uniform float movement;

void main()
{
    vec2 position = pos;
    position += movement/50;
    position.x /= aspectRatio;
    vec2 offset = vec2(0.4f,0.4f);
    gl_Position = vec4(position , 0f,1.0f);
}