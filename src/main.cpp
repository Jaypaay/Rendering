#include "opengl-framework/opengl-framework.hpp"

int main()
{
    gl::init("TPs de Rendering");
    gl::maximize_window();

    auto const rectangle_mesh = gl::Mesh{{
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position2D{0}},
        .data   = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f
        },
    }},
    .index_buffer = {
        0,1,2,
        0,2,3
    }
}};

    while (gl::window_is_open())
    {
        glClearColor(0.f,0.f,1.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        gl::bind_default_shader();
        rectangle_mesh.draw();
    }
}