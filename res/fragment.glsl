#version 410

out vec4 out_color;
in vec3 vertex_position;
in vec2 uv;

void main()
{
    //UV
    out_color = vec4(uv.x, uv.y, 0., 1.);
    
    //out_color = vec4(vertex_position, 1.);
    //out_color = vec4(0.84, 0.23, 0.23, 1.0);
}