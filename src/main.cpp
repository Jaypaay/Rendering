#include "opengl-framework/opengl-framework.hpp"
#include "glm/ext/matrix_clip_space.hpp"

int main()
{
    gl::init("TPs de Rendering");
    gl::maximize_window();
    // Camera
    auto camera = gl::Camera{};
    // Camera inputs
    gl::set_events_callbacks({camera.events_callbacks()});
    
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
        glm::mat4 const view_matrix = camera.view_matrix();
        glm::mat4 const projection_matrix = glm::infinitePerspective(10.f, gl::framebuffer_aspect_ratio(), 0.1f);
        shader.set_uniform("matrix", projection_matrix * view_matrix);
    }
}