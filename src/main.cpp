#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);

    auto const shader = gl::Shader{{
    .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
}};

shader.bind();
shader.set_uniform("uniformValue", glm::vec2{1.f, 3.f});
shader.set_uniform("aspectRatio", gl::framebuffer_aspect_ratio());


auto const rectangle_mesh = gl::Mesh{{
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position2D{0}},
        .data   = {
            -0.5f, -0.5f, // Position2D du 1er sommet
            +0.5f, -0.5f, // Position2D du 2ème sommet
            +0.5f, +0.5f, // Position2D du 3ème sommet
            -0.5f, +0.5f  // Position2D du 4ème sommet
        },
    }},
    .index_buffer   = {
        0, 1, 2, // Indices du premier triangle : on utilise le 1er, 2ème et 3ème sommet
        0, 2, 3  // Indices du deuxième triangle : on utilise le 1er, 3ème et 4ème sommet
    },
}};

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.3f, 1.f); 
        //glClear(GL_COLOR_BUFFER_BIT); // 
        shader.bind();
        shader.set_uniform("movement", gl::time_in_seconds());
        rectangle_mesh.draw(); 
    }
}