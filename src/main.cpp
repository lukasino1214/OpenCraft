#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Client/Rendering/Buffers.h"
#include "Client/Rendering/Camera.h"
#include "Client/Rendering/Shader.h"
#include "Client/Rendering/VertexArray.h"
#include "Client/Voxel/ChunkManager.h"
#include <glm/gtx/string_cast.hpp>
#include <FastNoise/FastNoise.h>
#define TRACY_ENABLE
#include "Tracy/Tracy.hpp"
#include "glm/gtx/hash.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
    auto const src_str = [source]() {
        switch (source)
        {
            case GL_DEBUG_SOURCE_API: return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
            case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
            case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        }
    }();

    auto const type_str = [type]() {
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR: return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER: return "MARKER";
            case GL_DEBUG_TYPE_OTHER: return "OTHER";
        }
    }();

    auto const severity_str = [severity]() {
        switch (severity) {
            case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
            case GL_DEBUG_SEVERITY_LOW: return "LOW";
            case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
            case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        }
    }();
    std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;


bool block[16] = {};

bool testVoxel(const int& pos) {
    if(-1 < pos && pos < 16) {

        std::cout << "yes" << std::endl;
    } else {
        std::cout << "no" << std::endl;
    }
}


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenCraft", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(message_callback, nullptr);

    const std::vector<GLfloat> m_clear_color{ 0.2f, 0.2f, 0.2f, 1.0f };

    std::string vertexSource = R"(
        #version 450 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;

        out vec3 color;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
        color = aColor;
        }
    )";

    std::string fragmentSource = R"(
        #version 450 core
        out vec4 FragColor;

        in vec3 color;

        void main()
        {
        	FragColor = vec4(color, 1.0f);
        }
    )";

    //ShaderClass shader(vertexSource, fragmentSource);

    /*uint32_t fbo, tex, depthTex;

    glCreateTextures(GL_TEXTURE_2D, 1, &tex);

    glTextureParameteri(tex, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureStorage2D(tex, 1, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT);


    glCreateTextures(GL_TEXTURE_2D, 1, &depthTex);

    glTextureParameteri(depthTex, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(depthTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(depthTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(depthTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(depthTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureStorage2D(depthTex, 1, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    //glTextureSubImage2D(tex, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glCreateFramebuffers(1, &fbo);

    glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, tex, 0);
    //glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, depthTex, 0);

    if(glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "framebuffer error\n";*/

    std::vector<float> ver = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f
    };

    std::vector<uint32_t> ind = {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
    };

    uint32_t  m_vbo, m_ibo, m_vao;

    glCreateBuffers(1, &m_vbo);
    glNamedBufferStorage(m_vbo, ver.size() * sizeof(float), &ver[0], 0);

    glCreateBuffers(1, &m_ibo);
    glNamedBufferStorage(m_ibo, ind.size() * sizeof(uint32_t), &ind[0], 0);

    glCreateVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    struct vertex { glm::vec3 pos, nrm; };

    glEnableVertexAttribArray(0);

    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, pos));

    glVertexAttribBinding(0, 0);


    glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, 3 * sizeof(float));
    glVertexArrayElementBuffer(m_vao, m_ibo);

    auto OpenSimplex = FastNoise::New<FastNoise::OpenSimplex2>();
    auto FractalFBm = FastNoise::New<FastNoise::FractalFBm>();
    FractalFBm->SetSource(OpenSimplex);
    FractalFBm->SetGain(0.280f);
    FractalFBm->SetOctaveCount(4);
    FractalFBm->SetLacunarity(2.0f);

    auto DomainScale = FastNoise::New<FastNoise::DomainScale>();
    DomainScale->SetSource(FractalFBm);
    DomainScale->SetScale(0.76f);

    auto PosationOutput = FastNoise::New<FastNoise::PositionOutput>();
    PosationOutput->Set<FastNoise::Dim::Y>(3.72f);

    auto add = FastNoise::New<FastNoise::Add>();
    add->SetLHS(DomainScale);
    add->SetRHS(PosationOutput);

    ChunkManager manager(add);

    for(int x = -4; x < 5; x++) {
        for(int y = -1; y < 1; y++) {
            for(int z = -4; z < 5; z++) {
                manager.AddChunk({x,y,z});
            }
        }
    }

    /*unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    std::string path = "res/Textures/container.jpg";
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);*/

    uint32_t array;
    glGenTextures( 1, &array );
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, array);

  //Create storage for the texture. (100 layers of 1x1 texels)
    glTexStorage3D( GL_TEXTURE_2D_ARRAY,
                  1,                    //No mipmaps as textures are 1x1
                  GL_RGB8,              //Internal format
                  16, 16,                 //width,height
                  100                   //Number of layers
                );

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    std::string path = "res/Textures/1.jpg";
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexSubImage3D( GL_TEXTURE_2D_ARRAY,
                         0,                     //Mipmap number
                         0,0,0,                 //xoffset, yoffset, zoffset
                         16,16,1,                 //width, height, depth
                         GL_RGB,                //format
                         GL_UNSIGNED_BYTE,      //type
                         data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    path = "res/Textures/3.jpg";

    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexSubImage3D( GL_TEXTURE_2D_ARRAY,
                         0,                     //Mipmap number
                         0,0,1,                 //xoffset, yoffset, zoffset
                         16,16,1,                 //width, height, depth
                         GL_RGB,                //format
                         GL_UNSIGNED_BYTE,      //type
                         data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    path = "res/Textures/4.png";

    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexSubImage3D( GL_TEXTURE_2D_ARRAY,
                         0,                     //Mipmap number
                         0,0,2,                 //xoffset, yoffset, zoffset
                         16,16,1,                 //width, height, depth
                         GL_RGB,                //format
                         GL_UNSIGNED_BYTE,      //type
                         data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    Camera camera(SCR_WIDTH, SCR_HEIGHT, {18,18, 18});

    Shader sky("res/Shader/Atmo.vert", "res/Shader/Atmo.frag");
    sky.Unbind();
    Shader shader("res/Shader/Cube.vert", "res/Shader/Cube.frag");
    //Shader atmo("res/Shader/Cube.vert", "res/Shader/Cube.frag");
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        /*glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClearNamedFramebufferfv(fbo, GL_COLOR, 0, m_clear_color.data());*/
        glClearBufferfv(GL_COLOR, 0, m_clear_color.data());
        glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0, 0);
        camera.Inputs(window);

        //std::cout << glm::to_string(camera.GetPosition()) << std::endl;

        sky.Bind();
        sky.SetMat4("projection", camera.GetProjection());
        sky.SetMat4("view", camera.GetView());
        glm::mat4 model = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
        model = glm::scale(glm::mat4(1.0f), {500.0f, 500.0f, 500.0f});
        sky.SetMat4("model", model);
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        sky.Unbind();
        shader.Bind();
        shader.SetMat4("projection", camera.GetProjection());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, array);
        shader.SetInt("texture", array);
        //shader.SetFloat3("color", {0.9f, 0.9f, 0.9f});
        model = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
        shader.SetMat4("view", camera.GetView());
        manager.Render(shader);
        shader.Unbind();
        //chunk.Render(shader);
        /*glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBlitNamedFramebuffer(fbo, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, GL_COLOR_BUFFER_BIT, GL_LINEAR);*/

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    ZoneScoped;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};
