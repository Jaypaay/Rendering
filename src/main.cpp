#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut

    auto const triangle_mesh = gl::Mesh{{
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position2D{0 /*Index de l'attribut dans le shader, on en reparle juste après*/}},
        .data   = {
            -1.f, -1.f, // Position2D du 1er sommet
            +1.f, -1.f, // Position2D du 2ème sommet
             0.f, +1.f  // Position2D du 3ème sommet
        },
    }},
}};

    while (gl::window_is_open())
    {
        glClearColor(0.f,0.f,1.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        gl::bind_default_shader();
        triangle_mesh.draw();
    }
}