#include "opengl-framework/opengl-framework.hpp"

int main()
{
    gl::init("TPs de Rendering");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
    auto const shader = gl::Shader{{
    .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
}};

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
        shader.bind();
        shader.set_uniform("aspect_ratio", gl::framebuffer_aspect_ratio());
        shader.set_uniform("uniformValue", glm::vec2{1.f,3.f});
        shader.set_uniform("movement", gl::time_in_seconds());

        //TODO set fade
        //shader.set_uniform("colorFade", glm::vec4{1.0f,1.0f,1.0f, ALPHAVALUE});
        rectangle_mesh.draw();
    }
}