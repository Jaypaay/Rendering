#include "opengl-framework/opengl-framework.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "tiny_obj_loader.h"


auto load_mesh(std::filesystem::path const& path) -> gl::Mesh
{
    // On lit le fichier avec tinyobj
    auto reader = tinyobj::ObjReader{};
    reader.ParseFromFile(gl::make_absolute_path(path).string(), {});

    // On met tous les attributs dans un tableau
    auto vertices = std::vector<float>{};
    for (auto const& shape : reader.GetShapes())
    {
        for (auto const& idx : shape.mesh.indices)
        {
            // Position
            vertices.push_back(reader.GetAttrib().vertices[3 * idx.vertex_index + 0]);
            vertices.push_back(reader.GetAttrib().vertices[3 * idx.vertex_index + 1]);
            vertices.push_back(reader.GetAttrib().vertices[3 * idx.vertex_index + 2]);

            // UV
            vertices.push_back(reader.GetAttrib().texcoords[2 * idx.texcoord_index + 0]);
            vertices.push_back(reader.GetAttrib().texcoords[2 * idx.texcoord_index + 1]);

            // Normale
            vertices.push_back(reader.GetAttrib().normals[3 * idx.normal_index + 0]);
            vertices.push_back(reader.GetAttrib().normals[3 * idx.normal_index + 1]);
            vertices.push_back(reader.GetAttrib().normals[3 * idx.normal_index + 2]);
        };
    }
    //Return mesh here
    return gl::Mesh{{
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position3D{0}, gl::VertexAttribute::UV{1}, gl::VertexAttribute::Normal3D{2} ,}, .data = vertices
    }}
}};
}


int main()
{
    gl::init("TPs de Rendering");
    gl::maximize_window();
    // Camera
    auto camera = gl::Camera{};
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
    auto const shader = gl::Shader{{
    .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
}};

    auto const texture = gl::Texture{
        gl::TextureSource::File{ // Peut être un fichier, ou directement un tableau de pixels
            .path           = "res/boatTexture.jpg",
            .flip_y         = true, // Il n'y a pas de convention universelle sur la direction de l'axe Y. Les fichiers (.png, .jpeg) utilisent souvent une direction différente de celle attendue par OpenGL. Ce booléen flip_y est là pour inverser la texture si jamais elle n'apparaît pas dans le bon sens.
            .texture_format = gl::InternalFormat::RGBA8, // Format dans lequel la texture sera stockée. On pourrait par exemple utiliser RGBA16 si on voulait 16 bits par canal de couleur au lieu de 8. (Mais ça ne sert à rien dans notre cas car notre fichier ne contient que 8 bits par canal, donc on ne gagnerait pas de précision). On pourrait aussi stocker en RGB8 si on ne voulait pas de canal alpha. On utilise aussi parfois des textures avec un seul canal (R8) pour des usages spécifiques.
        },
        gl::TextureOptions{
            .minification_filter  = gl::Filter::Linear, // Comment on va moyenner les pixels quand on voit l'image de loin ?
            .magnification_filter = gl::Filter::Linear, // Comment on va interpoler entre les pixels quand on zoom dans l'image ?
            .wrap_x               = gl::Wrap::Repeat,   // Quelle couleur va-t-on lire si jamais on essaye de lire en dehors de la texture ?
            .wrap_y               = gl::Wrap::Repeat,   // Idem, mais sur l'axe Y. En général on met le même wrap mode sur les deux axes.
        }
    };

    auto render_target = gl::RenderTarget{gl::RenderTarget_Descriptor{
    .width          = gl::framebuffer_width_in_pixels(),
    .height         = gl::framebuffer_height_in_pixels(),
    .color_textures = {
        gl::ColorAttachment_Descriptor{
            .format  = gl::InternalFormat_Color::RGBA8,
            .options = {
                .minification_filter  = gl::Filter::NearestNeighbour, // On va toujours afficher la texture à la taille de l'écran,
                .magnification_filter = gl::Filter::NearestNeighbour, // donc les filtres n'auront pas d'effet. Tant qu'à faire on choisit le moins coûteux.
                .wrap_x               = gl::Wrap::ClampToEdge,
                .wrap_y               = gl::Wrap::ClampToEdge,
            },
        },
    },
    .depth_stencil_texture = gl::DepthStencilAttachment_Descriptor{
        .format  = gl::InternalFormat_DepthStencil::Depth32F,
        .options = {
            .minification_filter  = gl::Filter::NearestNeighbour,
            .magnification_filter = gl::Filter::NearestNeighbour,
            .wrap_x               = gl::Wrap::ClampToEdge,
            .wrap_y               = gl::Wrap::ClampToEdge,
        },
    },
}};

    // Camera & Render target callbacks
    gl::set_events_callbacks({
    camera.events_callbacks(),
    {.on_framebuffer_resized = [&](gl::FramebufferResizedEvent const& e) {
        render_target.resize(e.width_in_pixels, e.height_in_pixels);
    }},
});

    auto const rectangle_mesh = gl::Mesh{{
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position3D{0}, gl::VertexAttribute::UV{1}},
        .data   = {
            // REQUIRED TO PUT ALL DATA HERE FOR 3D POSITIONS
                -0.5f, -0.5f, -0.5f, 0, 0,
                0.5f, -0.5f, -0.5f, 1, 0,
                0.5f,  0.5f, -0.5f, 1, 1,
                -0.5f,  0.5f, -0.5f, 0, 1,
                -0.5f, -0.5f,  0.5f, 0, 0,
                0.5f, -0.5f,  0.5f, 1, 0,
                0.5f,  0.5f,  0.5f, 1, 1,
                -0.5f,  0.5f,  0.5f,  0, 1
        },
    }},
    .index_buffer = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 3, 7, 7, 4, 0,
        1, 5, 6, 6, 2, 1,
        0, 1, 5, 5, 4, 0,
        3, 2, 6, 6, 7, 3
    }
}};

    while (gl::window_is_open())
    {
    //render_target.render([&]() {
    //glClearColor(1.f, 0.f, 0.f, 1.f); // Dessine du rouge, non pas à l'écran, mais sur notre render target
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ... mettez tout votre code de rendu ici

        glClearColor(0.f,0.f,1.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     
        shader.bind();
        shader.set_uniform("aspect_ratio", gl::framebuffer_aspect_ratio());
        shader.set_uniform("uniformValue", glm::vec2{1.f,3.f});
        shader.set_uniform("movement", gl::time_in_seconds());
        shader.set_uniform("sampler", texture);
        shader.set_uniform("light_direction", glm::vec3(0.2, 0.3, -1));
        //TODO set fade
        //shader.set_uniform("colorFade", glm::vec4{1.0f,1.0f,1.0f, ALPHAVALUE});

        auto const boat = load_mesh("res/fourareen.obj");
        boat.draw();
        glm::mat4 const view_matrix = camera.view_matrix();
        glm::mat4 const rotation = glm::rotate(glm::mat4{1.f}, gl::time_in_seconds(), glm::vec3{0.f, 0.f, 1.f});
        glm::mat4 const translation = glm::translate(glm::mat4{1.f}, glm::vec3{0.f,1.f,0.f});
        //glm::mat4 const projection_matrix = glm::ortho(2.0f, 2.0f, 2.0f, 2.0f);
        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f, gl::framebuffer_aspect_ratio(), 0.001f);
        //Model matrix (rotation, translation)
        //shader.set_uniform("matrix", rotation * translation * projection_matrix * view_matrix);
        shader.set_uniform("matrix", projection_matrix * view_matrix);
        //});
    }
}